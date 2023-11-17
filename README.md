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
    * [Future development](#future-development)
    * [Implementation details and usage](#implementation-details-and-usage)
- [Installation](#installation)
- [Current State](#current-state)
- [Contribution](#contribution)
- [Support](#support)
- [License](#license)

## Overview

### Reasons

Our Java API serves as the cornerstone of our technology, and with our SDK, you can seamlessly
integrate it into any language, leveraging it as a native library for all desktop platforms.

### Benefits

- :rocket: Performance almost is same as for our best Java API
- :milky_way: Wider functionality
- :gemini: Identical programming interfaces to our best API
- :thumbsup: Higher quality of support and service

### Future development

Our team is committed to continuously improving the library by regularly releasing updates to our
Java API and implementing them in the native library.

### Implementation details and usage

**todo:** _text_

```C
#include <iostream>
#include <thread>

#include "api/Api.h"
#include "api/Subscription.h"
#include "api/Connection.h"

int main(int argc, char** argv) {
  // load cmd args  
  const auto javaHomePath = argv[1];
  const int vmOptionsSize = 1;
  const char* jvmArgs[vmOptionsSize] = { "-Xmx12G" };
  const auto address = argv[2];
  const auto symbol = argv[3];

  // init context, connection, subscription
  std::cout << "Connection to address:" << address << std::endl;
  dxfeed_init(javaHomePath, jvmArgs, vmOptionsSize);
  void* dxFeed = dxfeed_get_instance();
  auto connection = dxfeed_create_connection(dxFeed, address);
  auto subscription = dxfeed_create_subscription(connection, 0);

  // add listener with user code
  dxfeed_add_listener(subscription, [](const void *events, int count) {
    auto timeAndSaleList = reinterpret_cast<const TimeAndSale*>(events);
    for (int i = 0; i < count; ++i) {
      auto quote = std::make_shared<TimeAndSale>(timeAndSaleList[i]);
      // todo: work
    }
  });

  // add symbol to subscription
  dxfeed_add_symbol(subscription, symbol);
  
  // sleep then clean up
  std::chrono::hours hours(24); 
  std::this_thread::sleep_for(hours);
  delete (dxfeed::Subscription*)subscription;
  delete (dxfeed::Connection*) connection;
}
```

There are the following usage examples:

* [dxEndpoint Subscription](src/main/c/main.cpp)

## Installation

### Build on local machine

```
$ git clone https://github.com/dxFeed/dxfeed-jni-native-sdk.git
$ cd dxfeed-jni-native-sdk
$ mvn clean package
$ cd native
$ sh build_release.sh
$ cd build
$ ./native_app $JAVA_HOME 127.0.0.1:6666 ETH/USD:GDAX -Xmx12G
```

### Artifacts build by CI:

**todo:** _setup github CI_

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

## Current State

### Event Types

- [x] [TimeAndSale](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TimeAndSale.html)
  represents a trade or
  other market event with price, like market open/close price, etc.
- [x] [AnalyticOrder](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/AnalyticOrder.html) represents an
  extension of Order introducing analytic information, e.g., adding iceberg-related
  information to this order
- [x] [Candle](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/Candle.html) event with
  open, high, low, and
  close prices and other information for a specific period
- [x] [Configuration](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/misc/Configuration.html) is an event with an
  application-specific attachment
- [x] [Greeks](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Greeks.html) is a snapshot
  of the option
  price, Black-Scholes volatility, and Greeks
- [x] [Message](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/misc/Message.html) is an event
  with an
  application-specific attachment
- [x] [OptionSale](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/OptionSale.html) is a
  represents a trade or another market event with the price (for example, market open/close price,
  etc.) for each option symbol listed under the specified Underlying.
- [x] [Order](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Order.html) is a snapshot
  of the full available
  market depth for a symbol
- [x] [Profile](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Profile.html) is a
  snapshot that contains the
  security instrument description
- [x] [Quote](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Quote.html) is a snapshot
  of the best bid and
  ask prices and other fields that change with each quote
- [x] [Series](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Series.html) is a snapshot
  of computed values
  available for all options series for a given underlying symbol based on options market prices
- [x] [SpreadOrder](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/SpreadOrder.html) is
  a snapshot of the
  full available market depth for all spreads
- [x] [Summary](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Summary.html) is a
  snapshot of the trading
  session, including session highs, lows, etc.
- [x] [TheoPrice](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/TheoPrice.html) is a
  snapshot of the
  theoretical option price computation that is periodically performed
  by [dxPrice](http://www.devexperts.com/en/products/price.html) model-free computation
- [x] [Trade](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Trade.html) is a snapshot
  of the price and size
  of the last trade during regular trading hours and an overall day
  volume and day turnover
- [x] [TradeETH](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TradeETH.html) is a
  snapshot of the price
  and size of the last trade during extended trading hours and the extended
  trading hours day volume and day turnover
- [x] [Underlying](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Underlying.html) is a
  snapshot of computed
  values available for an option underlying symbol based on the market’s option prices

### Subscription Symbols

- [x] String
- [x] [CandleSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/CandleSymbol.html) symbol used
  with [DXFeedSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html)
  class to subscribe for [Candle](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/Candle.html) events
- [x] [IndexedSubscriptionSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/IndexedEventSubscriptionSymbol.html)
  represents subscription to a specific source of indexed events
- [x] [TimeSeriesSubscriptionSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/TimeSeriesSubscriptionSymbol.html) 
  represents subscription to time-series events
- [x] [WildcardSymbol.ALL](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/WildcardSymbol.html)   represents a
  *wildcard* subscription to all events of the specific event type

### Subscriptions & Models

- [x] [CreateSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html)
  creates a new
  subscription for multiple event types *attached* to a specified feed
- [x] [CreateTimeSeriesSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedTimeSeriesSubscription.html)
  extends DXFeedSubscription to conveniently subscribe to time series of events for a set of symbols
  and event
  types ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/api/DXFeedConnect.java))
- [x] [GetLastEvent](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEvent-E-)
  returns the last
  event for the specified event
  instance ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/api/DXFeedSample.java))
- [x] [GetLastEvents](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEvents-java.util.Collection-)
  returns the last events for the specified event instances list
- [ ] [GetLastEventPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventPromise-java.lang.Class-java.lang.Object-)
  requests the last event for the specified event type and
  symbol ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/console/LastEventsConsole.java))
- [ ] [GetLastEventsPromises](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventsPromises-java.lang.Class-java.util.Collection-)
  requests the last events for the specified event type and symbol collection
- [x] [GetLastEventIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventIfSubscribed-java.lang.Class-java.lang.Object-)
  returns the last event for the specified event type and symbol if there’s a subscription for it
- [ ] [GetIndexedEventsPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getIndexedEventsPromise-java.lang.Class-java.lang.Object-com.dxfeed.event.IndexedEventSource-)
  requests an indexed events list for the specified event type, symbol, and source
- [x] [GetIndexedEventsIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getIndexedEventsIfSubscribed-java.lang.Class-java.lang.Object-com.dxfeed.event.IndexedEventSource-)
  requests an indexed events list for the specified event type, symbol, and source if there’s a
  subscription for it
- [ ] [GetTimeSeriesPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getTimeSeriesPromise-java.lang.Class-java.lang.Object-long-long-)
  requests time series of events for the specified event type, symbol, and time
  range ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/_simple_/FetchDailyCandles.java))
- [x] [GetTimeSeriesIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getTimeSeriesIfSubscribed-java.lang.Class-java.lang.Object-long-long-)
  requests time series of events for the specified event type, symbol, and time range if there’s a
  subscription for it
- [ ] [TimeSeriesEventModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/TimeSeriesEventModel.html) -
  is a model
  of a list of time series
  events ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ui/swing/DXFeedCandleChart.java))
- [ ] [IndexedEventModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/IndexedEventModel.html)
  is a model of a
  list of indexed
  events ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ui/swing/DXFeedTimeAndSales.java))
- [ ] [OrderBookModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/market/OrderBookModel.html)
  is a model of
  convenient Order Book
  management ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ui/swing/DXFeedMarketDepth.java))

### IPF & Schedule
- [ ] [InstrumentProfile](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/InstrumentProfile.html)
  represents basic
  profile information about a market
  instrument ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ipf/DXFeedIpfConnect.java))
- [ ] [InstrumentProfileCollector](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/live/InstrumentProfileCollector.html)
  collects instrument profile updates and provides the live instrument profiles
  list ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ipf/DXFeedLiveIpfSample.java))
- [ ] [Schedule](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/schedule/Schedule.html) provides API
  to retrieve and
  explore various exchanges’ trading schedules and different financial instrument
  classes ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/schedule/ScheduleSample.java))

### Services

- [ ] [OnDemandService](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ondemand/OnDemandService.html)
  provides on-demand
  historical tick data replay
  controls ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/ondemand/OnDemandSample.java))

### Endpoint Roles
- [x] [FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#FEED) connects
  to the remote data
  feed provider and is optimized for real-time or delayed data processing
- [x] [STREAM_FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#STREAM_FEED)
  is similar to
  FEED and also connects to the remote data feed provider but is designed for bulk data parsing from
  files
- [x] [LOCAL_HUB](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#LOCAL_HUB)
  is a local hub
  without the ability to establish network connections. Events published via publisher are delivered
  to local feed only.
- [x] [PUBLISHER](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#PUBLISHER)
  connects to the
  remote publisher hub (also known as multiplexor) or creates a publisher on the local
  host ([Java API sample](https://github.com/devexperts/QD/blob/master/dxfeed-samples/src/main/java/com/dxfeed/sample/_simple_/WriteTapeFile.java))
- [x] [STREAM_PUBLISHER](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#STREAM_PUBLISHER)
  is
  similar to PUBLISHER and also connects to the remote publisher hub, but is designed for bulk data
  publishing
- [ ] [ON_DEMAND_FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#ON_DEMAND_FEED)
  is similar
  to FEED, but it is designed to be used with OnDemandService for historical data replay only

## Contribution

[Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)

[Semantic Versioning](https://semver.org/)

## Support

Our support team on
our [customer portal](https://jira.in.devexperts.com/servicedesk/customer/portal/1/create/122) is
ready to answer any questions and help with the transition.

## License

MPL-2.0
