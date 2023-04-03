# dxfeed-jni-native-sdk

### How to run

```
$ git clone https://github.com/dxFeed/dxfeed-jni-sdk.git
$ cd dxfeed-jni-sdk
```

This package grants you access to [dxFeed market data](https://dxfeed.com/market-data/). The library
is designed as a С-library and was
compiled using [Java Native Interface](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/)
and our flagship [dxFeed Java API](https://docs.dxfeed.com/dxfeed/api/overview-summary.html), making
it easily integrable
into your projects.

![Platform](https://img.shields.io/badge/platform-win--x64%20%7C%20linux--x64%20%7C%20osx-lightgrey)

[![License](https://img.shields.io/badge/license-MPL--2.0-orange)](https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/master/LICENSE)

## Table of Contents

- [Overview](#overview)
    * [Reasons](#reasons)
    * [Implementation details and usage](#Implementation-details-and-usage)
- [Installation](#installation)
- [Current State](#current-state)
- [Contribution](#Contribution)
- [License](#License)

## Overview

### Reasons

Our Java API serves as the cornerstone of our technology, and with our SDK, you can seamlessly
integrate it into any language, leveraging it as a native library for all desktop platforms.

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

* [dxEndpoint Subscription](native/main.cpp)

## Installation

You can find artifacts here:

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

#### Ready
- :white_check_mark: [TimeAndSale](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TimeAndSale.html) -
  represents a trade or
  other market event with price, like market open/close price, etc.

#### Not ready yet
- :x: [AnalyticOrder](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/AnalyticOrder.html)
- :x: [Candle](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/Candle.html)
- :x: [Configuration](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/misc/Configuration.html)
- :x: [Greeks](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Greeks.html)
- :x: [Message](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/misc/Message.html)
- :x: [OptionSale](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/OptionSale.html)
- :x: [Order](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Order.html)
- :x: [Profile](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Profile.html)
- :x: [Quote](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Quote.html)
- :x: [Series](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Series.html)
- :x: [SpreadOrder](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/SpreadOrder.html)
- :x: [Summary](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Summary.html)
- :x: [TheoPrice](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/TheoPrice.html)
  by [dxPrice](http://www.devexperts.com/en/products/price.html) model-free computation
- :x: [Trade](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Trade.html)
- :x: [TradeETH](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TradeETH.html)
- :x: [Underlying](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Underlying.html)

### Subscription Symbols

#### Ready
- :white_check_mark: String

#### Not ready yet
- :x: [TimeSeriesSubscriptionSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/TimeSeriesSubscriptionSymbol.html)
- :x: [IndexedSubscriptionSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/IndexedEventSubscriptionSymbol.html)
- :x: [WildcardSymbol.ALL](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/osub/WildcardSymbol.html)
- :x: [CandleSymbol](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/CandleSymbol.html) 

### Subscriptions & Models

#### Ready
- :white_check_mark: [CreateSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html)
  creates a new
  subscription for multiple event types *attached* to a specified feed

#### Not ready yet
- :x: [CreateTimeSeriesSubscription](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedTimeSeriesSubscription.html)
- :x: [GetLastEvent](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEvent-E-)
- :x: [GetLastEvents](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEvents-java.util.Collection-)
- :x: [GetLastEventPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventPromise-java.lang.Class-java.lang.Object-)
- :x: [GetLastEventsPromises](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventsPromises-java.lang.Class-java.util.Collection-)
- :x: [GetLastEventIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getLastEventIfSubscribed-java.lang.Class-java.lang.Object-)
- :x: [GetIndexedEventsPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getIndexedEventsPromise-java.lang.Class-java.lang.Object-com.dxfeed.event.IndexedEventSource-)
- :x: [GetIndexedEventsIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getIndexedEventsIfSubscribed-java.lang.Class-java.lang.Object-com.dxfeed.event.IndexedEventSource-)
- :x: [GetTimeSeriesPromise](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getTimeSeriesPromise-java.lang.Class-java.lang.Object-long-long-)
- :x: [GetTimeSeriesIfSubscribed](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html#getTimeSeriesIfSubscribed-java.lang.Class-java.lang.Object-long-long-)
- :x: [TimeSeriesEventModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/TimeSeriesEventModel.html) -
- :x: [IndexedEventModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/IndexedEventModel.html)
- :x: [OrderBookModel](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/model/market/OrderBookModel.html)

### IPF & Schedule [not ready yet]

- :x: [InstrumentProfile](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/InstrumentProfile.html)
- :x: [InstrumentProfileCollector](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ipf/live/InstrumentProfileCollector.html)
- :x: [Schedule](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/schedule/Schedule.html)

### Services [not ready yet]

- :x: [OnDemandService](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/ondemand/OnDemandService.html)

### Endpoint Roles
#### Ready
- :white_check_mark: [FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#FEED) connects
  to the remote data
  feed provider and is optimized for real-time or delayed data processing

#### Not ready yet

- :x: [STREAM_FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#STREAM_FEED)
- :x: [LOCAL_HUB](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#LOCAL_HUB)
- :x: [PUBLISHER](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#PUBLISHER)
- :x: [STREAM_PUBLISHER](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#STREAM_PUBLISHER)
- :x: [ON_DEMAND_FEED](https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html#ON_DEMAND_FEED)

## Contribution

[Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/)

[Semantic Versioning](https://semver.org/)

## License

MPL-2.0