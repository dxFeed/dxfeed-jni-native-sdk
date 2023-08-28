<picture>
 <source media="(prefers-color-scheme: dark)" srcset="docs/images/logo_dark.svg">
 <img alt="light" src="docs/images/logo_light.svg">
</picture>

This package grants you access to [dxFeed market data](https://dxfeed.com/market-data/). The library
is designed as a С-library and directly invokes
our flagship [dxFeed Java API](https://docs.dxfeed.com/dxfeed/api/overview-summary.html) 
using [Java Native Interface](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/), making
it easily integrable into your projects.

:warning: It’s an **alpha** version and still under active development. **Don’t use it in a production environment.**

![Platform](https://img.shields.io/badge/platform-win--x64%20%7C%20linux--x64%20%7C%20osx--64%20%7C%20osx--arm64-green)
[![License](https://img.shields.io/badge/license-MPL--2.0-orange)](LICENSE)

## Table of Contents

- [Overview](#overview)
    * [Reasons](#reasons)
    * [Benefits](#benefits)
    * [Implementation details](#implementation-details)
    * [Usage (demo)](#usage-demo)
- [Installation](#installation)
    * [Build JAR dependency](#build-jar-dependency)
    * [Build Native depencdecy](#build-native-dependency)
      * [Unix systems](#unix-systems)
      * [Windows x64](#windows-x64)
    * [Build Demo with Native dependency](#build-demo-with-native-dependency)
      * [Unix systems](#unix-systems)
      * [Windows x64](#windows-x64)
- [Samples](#samples)
    * [How to build a sample](#how-to-build-a-sample)   
- [Current State](#current-state)
    * [Endpoint roles](#endpoint-roles)
    * [Event types](#event-types)
    * [Subscription Symbols](#subscription-symbols)
    * [Subscriptions & models](#subscriptions--models)
    * [IPF & Schedule](#ipf--schedule)
    * [Services](#services)

## Overview

### Reasons

Our Java API serves as the cornerstone of our technology, and with our SDK, you can seamlessly
integrate it into any language, leveraging it as a native library for all desktop platforms.

### Benefits

- :rocket: Performance almost is same as for our best Java API
- :milky_way: Wider functionality
- :gemini: Identical programming interfaces to our best API
- :thumbsup: Higher quality of support and service


### Implementation details

We use standalone [JDK](https://openjdk.org/) and specially written code that invokes Java methods from native using [Java Native Iterface](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/intro.html) to get dynamically linked libraries for different platforms (Linux, macOS, and Windows) based on the [latest Java API package](https://dxfeed.jfrog.io/artifactory/maven-open/com/devexperts/qd/dxfeed-api/).  

Then, the resulting dynamic link library (`DxFeedJniNativeSdk.dylib|.so|.dll`) is used through C ABI (application binary interface), and we write programming interfaces that describe our business model (similar to Java API).

As a result, we get a full-featured, similar performance as with Java API. Regardless of the language, writing the final application logic using API calls will be very similar (only the syntax will be amended, "best practices", specific language restrictions).

<img width="436" alt="image" src="https://github.com/dxFeed/dxfeed-jni-native-sdk/assets/4503006/5d642f7c-a03d-4a7c-b21d-dda07a8ad74d">




* Required components:
  * preinstalled `Java VM 1.8+`
  * `JAR dependency` with:
    * latest Java API package [Java API](https://dxfeed.jfrog.io/artifactory/maven-open/com/devexperts/qd/dxfeed-api/) 
    * some [Java wrappers](src/main/java/com/dxfeed/api) for C ABI entry points
  * C ABI implementation as `dynamic link library` (`DxFeedJniNativeSdk.dylib|.so|.dll`)

### Usage (demo)

<details>
    <summary>Code</summary>

   ```C
   #include <iostream>
   #include <chrono>
   #include <thread>
   
   #include "api/dxfg_api.h"
   
   void c_print(graal_isolatethread_t* thread, dxfg_event_type_list* events, void* user_data) {
     for (int i = 0; i < events->size; ++i) {
       dxfg_event_type_t* pEvent = events->elements[i];
       if (pEvent && pEvent->clazz == DXFG_EVENT_QUOTE) {
         const auto* quote = (const dxfg_quote_t*) pEvent;
         printf(
           "C: QUOTE{event_symbol=%s, bid_price=%f, bid_time=%lld, ask_price=%f, ask_time=%lld}\n",
           quote->market_event.event_symbol,
           quote->bid_price,
           quote->bid_time,
           quote->ask_price,
           quote->ask_time);
       }
     }
   }
   
   int main() {
     const auto address = "demo.dxfeed.com:7300";
     const auto symbol = "AAPL";
   
     auto thread = create_thread();
     if (thread != nullptr) {
       // Create endpoint and connect to specified address.
       dxfg_endpoint_t* endpoint = dxfg_DXEndpoint_create(thread);
       dxfg_DXEndpoint_connect(thread, endpoint, address);
   
       // Create feed and subscription with specified types attached to feed.
       dxfg_feed_t* feed = dxfg_DXEndpoint_getFeed(thread, endpoint);
       dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_QUOTE);
   
       // Adds event listener.
       dxfg_feed_event_listener_t* listener = dxfg_DXFeedEventListener_new(thread, &c_print, nullptr);
       dxfg_DXFeedSubscription_addEventListener(thread, subscription, listener);
   
       // Create STRING symbol.
       dxfg_string_symbol_t stringSymbol;
       stringSymbol.supper.type = STRING;
       stringSymbol.symbol = symbol;
   
       // Add symbol to subscription.
       dxfg_DXFeedSubscription_setSymbol(thread, subscription, &stringSymbol.supper);
   
       // Sleep 10 seconds waiting for the response about Quotes.
       std::chrono::seconds seconds(10);
       std::this_thread::sleep_for(seconds);
   
       // Close subscription and clear resources.
       dxfg_DXFeedSubscription_close(thread, subscription);
       dxfg_DXEndpoint_close(thread, endpoint);
       dxfg_JavaObjectHandler_release(thread, &listener->handler);
       dxfg_JavaObjectHandler_release(thread, &subscription->handler);
       dxfg_JavaObjectHandler_release(thread, &endpoint->handler);
   }
   ```
</details>

## Installation

### Build JAR dependency
  
  * artifact `src/c/main/demo/build/dxfeed-jni-native-sdk-0.1.0.jar`

```
$ git clone https://github.com/dxFeed/dxfeed-jni-native-sdk.git
$ cd dxfeed-jni-native-sdk
$ mvn clean package
```

### Build Native dependency

#### Unix systems
  
  * artifact `src/c/main/jni-lib/build/DxFeedJniNativeSdk.dylib|.so`
   
```
$ cd src/main/c/jni-lib
$ sh build_release.sh
```
   
#### Windows x64

  * artifact `src/c/main/jni-lib/build/DxFeedJniNativeSdk.dll`

```
$ cd src\main\c\jni-lib
$ build_release.cmd
```    

### Build Demo with Native dependency

#### Unix systems
  * artifact `src/c/main/demo/build/dxfeed-jni-native-sdk-0.1.0.jar`
  * artifact `src/c/main/demo/build/DxFeedJniNativeSdk.dylib|.so`
  * artifact `src/c/main/demo/build/DxFeedDemo`
   
```
$ cd src/main/c
$ sh build_demo_release.sh
$ cd demo/build
$ ./DxFeedDemo demo.dxfeed.com:7300 AAPL
```
   
#### Windows x64
  * artifact `src\c\main\demo\build\dxfeed-jni-native-sdk-0.1.0.jar`
  * artifact `src\c\main\demo\build\DxFeedJniNativeSdk.dll`
  * artifact `src\c\main\demo\build\DxFeedDemo.exe`

```
$ cd src\main\c
$ build_demo_release.cmd
$ cd demo\build
$ DxFeedDemo.exe demo.dxfeed.com:7300 AAPL
```


## Documentation

Find useful information in our self-service dxFeed Knowledge Base:

- [dxFeed Knowledge Base](https://kb.dxfeed.com/index.html?lang=en)
    * [Getting Started](https://kb.dxfeed.com/en/getting-started.html)
    * [Troubleshooting](https://kb.dxfeed.com/en/troubleshooting-guidelines.html)
    * [Market Events](https://kb.dxfeed.com/en/data-model/dxfeed-api-market-events.html)
    * [Event Delivery contracts](https://kb.dxfeed.com/en/data-model/model-of-event-publishing.html#event-delivery-contracts)
    * [dxFeed API Event classes](https://kb.dxfeed.com/en/data-model/model-of-event-publishing.html#dxfeed-api-event-classes)
    * [Exchange Codes](https://kb.dxfeed.com/en/data-model/exchange-codes.html)
    * [Symbology Guide](https://kb.dxfeed.com/en/data-model/symbology-guide.html)

## Samples

- [x] [Demo](src/main/c/demo/demo.cpp) is a sample demontration (linked with DxFeedJniNativeSdk) how to connect to address, add event listener and subscribe to `Quote` event
- [x] [DxFeedConnect](src/main/c/samples/DxFeedConnect/main.cpp) is a sample demontration how to connect to use console args to connect address and subscribe to `Quote` event
- [x] [ConvertTapeFile](src/main/c/samples/ConvertTapeFile/main.cpp)
  demonstrates how to convert one tape file to another tape file with optional intermediate processing or filtering
- [x] [DxFeedFileParser](src/main/c/samples/DxFeedFileParser/main.cpp)
  is a simple demonstration of how events are read form a tape file
- [x] [DxFeedSample](src/main/c/samples/DxFeedSample/main.cpp)
  is a simple demonstration of how to create multiple event listeners and subscribe to `Quote` and `Trade` events, using a `DxFeed` instance singleton and `dxfeed.properties` file
- [x] [PrintQuoteEvents](src/main/c/samples/PrintQuoteEvents/main.cpp)
  is a simple demonstration of how to subscribe to the `Quote` event, using a `DxFeed` instance singleton and `dxfeed.properties` file
- [x] [WriteTapeFile](src/main/c/samples/WriteTapeFile/main.cpp)
  is a simple demonstration of how to write events to a tape file
- [ ] DxFeedPublishProfiles is a simple demonstration of how to publish market events
- [ ] ScheduleSample is a simple demonstration of how to get various scheduling information for instruments

### How to build a sample
* [build JAR dependency](#build-jar-dependency) -> `src/c/main/demo/build/dxfeed-jni-native-sdk-0.1.0.jar`
* [build native dependency](#build-native-dependency) -> `src/c/main/demo/build/DxFeedJniNativeSdk.dylib|.so|.dll`
* copy to `${SAMPLE_DIR}/build` files
  * `dxfeed-jni-native-sdk-0.1.0.jar`
  * `DxFeedJniNativeSdk.dylib|.so|.dll`
* `cd ${SAMPLE_DIR}`
* run script `sh build_release.sh` or `build_release.cmd`
  * or open `CMakeLists.txt` as project and build from IDE

## Current State

### Endpoint Roles

- [x] [FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#FEED)
  connects to the remote data feed provider and is optimized for real-time or delayed data processing,
  **this is a default role**
  ([.NET API sample](https://github.com/dxFeed/dxfeed-graal-net-api/blob/main/samples/DxFeedConnect/Program.cs))

- [x] [STREAM_FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#STREAM_FEED)
  is similar to `Feed` and also connects to the remote data feed provider but is designed for bulk data parsing from
  files
  ([.NET API sample](https://github.com/dxFeed/dxfeed-graal-net-api/blob/main/samples/DxFeedFileParser/Program.cs))

- [x] [PUBLISHER](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#PUBLISHER)
  connects to the remote publisher hub (also known as multiplexor) or creates a publisher on the local host
  ([.NET API sample](https://github.com/dxFeed/dxfeed-graal-net-api/blob/main/samples/WriteTapeFile/Program.cs))

- [x] [STREAM_PUBLISHER](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#STREAM_PUBLISHER)
  is similar to `Publisher` and also connects to the remote publisher hub, but is designed for bulk data publishing
  ([.NET API sample](https://github.com/dxFeed/dxfeed-graal-net-api/blob/main/samples/ConvertTapeFile/Program.cs))

- [x] [LOCAL_HUB](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#LOCAL_HUB)
  is a local hub without the ability to establish network connections. Events published via `Publisher` are delivered to
  local `Feed` only

- [ ] [ON_DEMAND_FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#ON_DEMAND_FEED)
  is similar to `Feed`, but it is designed to be used
  with  [OnDemandService](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ondemand/OnDemandService.html) for historical
  data replay only

### Event Types

- [x] [Order](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Order.html)
  is a snapshot of the full available market depth for a symbol

- [x] [SpreadOrder](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/SpreadOrder.html)
  is a snapshot of the full available market depth for all spreads

- [x] [AnalyticOrder](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/AnalyticOrder.html)
  is an `Order` extension that introduces analytic information, such as adding iceberg-related information to a given
  order

- [x] [Trade](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Trade.html)
  is a snapshot of the price and size of the last trade during regular trading hours and an overall day volume and day
  turnover

- [x] [TradeETH](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TradeETH.html)
  is a snapshot of the price and size of the last trade during extended trading hours and the extended trading hours day
  volume and day turnover

- [x] [Candle](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/Candle.html)
  event with open, high, low, and close prices and other information for a specific period

- [x] [Quote](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Quote.html)
  is a snapshot of the best bid and ask prices and other fields that change with each quote

- [x] [Profile](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Profile.html)
  is a snapshot that contains the security instrument description

- [x] [Summary](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Summary.html)
  is a snapshot of the trading session, including session highs, lows, etc.

- [x] [TimeAndSale](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TimeAndSale.html)
  represents a trade or other market event with price, such as the open/close price of a market, etc.

- [x] [Greeks](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Greeks.html)
  is a snapshot of the option price, Black-Scholes volatility, and greeks

- [x] [Series](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Series.html)
  is a snapshot of computed values available for all options series for a given underlying symbol based on options
  market prices

- [x] [TheoPrice](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/TheoPrice.html)
  is a snapshot of the theoretical option price computation that is periodically performed
  by [dxPrice](http://www.devexperts.com/en/products/price.html) model-free computation

- [x] [Underlying](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Underlying.html)
  is a snapshot of computed values available for an option underlying symbol based on the market’s option prices

- [x] [OptionSale](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/OptionSale.html)
  represents a trade or another market event with the price (for example, market open/close price, etc.) for each option
  symbol listed under the specified `Underlying`

- [x] [Configuration](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/misc/Configuration.html)
  is an event with an application-specific attachment

- [x] [Message](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/misc/Message.html)
  is an event with an application-specific attachment

### Subscription Symbols

- [x] [String](https://learn.microsoft.com/en-us/dotnet/api/system.string?view=net-6.0)
  is a string representation of the symbol

- [x] [TimeSeriesSubscriptionSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/TimeSeriesSubscriptionSymbol.html)
  represents subscription to time-series events

- [x] [IndexedEventSubscriptionSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/IndexedEventSubscriptionSymbol.html)
  represents subscription to a specific source of indexed events

- [x] [WildcardSymbol.ALL](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/WildcardSymbol.html)
  represents a  *wildcard* subscription to all events of the specific event type

- [x] [CandleSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/CandleSymbol.html)
  is a symbol used with [DXFeedSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html)
  class to subscribe for [Candle](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/Candle.html) events

### Subscriptions & Models

- [x] [DXFeedSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html)
  is a subscription for a set of symbols and event types

- [x] [DXFeedTimeSeriesSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedTimeSeriesSubscription.html)
  extends `DXFeedSubscription` to conveniently subscribe to time series events for a set of symbols and event types
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/api/DXFeedConnect.java))

- [ ] [ObservableSubscription](https://github.com/devexperts/QD/blob/master/dxfeed-api/src/main/java/com/dxfeed/api/osub/ObservableSubscription.java)
  is an observable set of subscription symbols for the specific event
  type ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/_simple_/PublishProfiles.java))

- [x] [GetLastEvent](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEvent-E-)
  returns the last event for the specified event instance
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/api/DXFeedSample.java))

- [x] [GetLastEvents](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEvents-java.util.Collection-)
  returns the last events for the specified event instances list

- [ ] [GetLastEventPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventPromise-java.lang.Class-java.lang.Object-)
  requests the last event for the specified event type and symbol
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/console/LastEventsConsole.java))

- [ ] [GetLastEventsPromises](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventsPromises-java.lang.Class-java.util.Collection-)
  requests the last events for the specified event type and symbol collection

- [x] [GetLastEventIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventIfSubscribed-java.lang.Class-java.lang.Object-)
  returns the last event for the specified event type and symbol if there’s a subscription for it

- [ ] [GetIndexedEventsPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getIndexedEventsPromise-java.lang.Class-java.lang.Object-com.dxfeed.event.IndexedEventSource-)
  requests an indexed events list for the specified event type, symbol, and source

- [x] [GetIndexedEventsIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getIndexedEventsIfSubscribed-java.lang.Class-java.lang.Object-com.dxfeed.event.IndexedEventSource-)
  returns a list of indexed events for the specified event type, symbol, and source, if there’s a subscription for it

- [ ] [GetTimeSeriesPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getTimeSeriesPromise-java.lang.Class-java.lang.Object-long-long-)
  requests time series events for the specified event type, symbol, and time range
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/_simple_/FetchDailyCandles.java))

- [x] [GetTimeSeriesIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getTimeSeriesIfSubscribed-java.lang.Class-java.lang.Object-long-long-)
  requests time series events for the specified event type, symbol, and time range if there’s a subscription for it

- [ ] [TimeSeriesEventModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/TimeSeriesEventModel.html)
  is a model of a list of time series events
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ui/swing/DXFeedCandleChart.java))

- [ ] [IndexedEventModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/IndexedEventModel.html)
  is an indexed event list model
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ui/swing/DXFeedTimeAndSales.java))

- [ ] [OrderBookModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/market/OrderBookModel.html)
  is a model of convenient Order Book management
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ui/swing/DXFeedMarketDepth.java))

### IPF & Schedule

- [ ] [InstrumentProfile](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/InstrumentProfile.html)
  represents basic profile information about a market instrument
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ipf/DXFeedIpfConnect.java))

- [ ] [InstrumentProfileReader](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/InstrumentProfileReader.html) reads
  instrument profiles from the stream using Instrument Profile Format (IPF)

- [ ] [InstrumentProfileCollector](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/live/InstrumentProfileCollector.html)
  collects instrument profile updates and provides the live instrument profiles list
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ipf/DXFeedLiveIpfSample.java))

- [ ] [InstrumentProfileConnection](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/live/InstrumentProfileConnection.html)
  connects to an instrument profile URL and reads instrument profiles with support of streaming live updates

- [ ] [Schedule](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/schedule/Schedule.html)
  provides an API to retrieving and exploring the trading schedules of various exchanges and different financial
  instrument classes
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/schedule/ScheduleSample.java))

- [ ] [Option Series](https://github.com/devexperts/QD/blob/master/dxfeed-api/src/main/java/com/dxfeed/ipf/option/OptionSeries.java)
  is a series of call and put options with different strike sharing the same attributes of expiration, last trading day,
  spc, multiplies,
  etc. ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ipf/option/DXFeedOptionChain.java))

### Services

- [ ] [OnDemandService](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ondemand/OnDemandService.html)
  provides on-demand historical tick data replay controls
  ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ondemand/OnDemandSample.java))
