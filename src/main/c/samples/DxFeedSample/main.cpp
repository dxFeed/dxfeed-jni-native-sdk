// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include "api/dxfg_api.h"

class Resources {
  graal_isolatethread_t* thread = nullptr;
  dxfg_subscription_t* subscription = nullptr;
  dxfg_feed_event_listener_t* listenerWrapper = nullptr;
public:
  Resources(graal_isolatethread_t* t, dxfg_subscription_t* sub, dxfg_feed_event_listener_t* eventListener) :
    thread(t),
    subscription(sub),
    listenerWrapper(eventListener)
  {}

  ~Resources() {
    if (subscription) {
      std::cout << "Resources close and clear subscription" << std::endl;
      dxfg_DXFeedSubscription_close(thread, subscription);
      dxfg_JavaObjectHandler_release(thread, &subscription->handler);
    }
    if (listenerWrapper) {
      std::cout << "Resources clear listener wrapper" << std::endl;
      dxfg_JavaObjectHandler_release(thread, &listenerWrapper->handler);
    }
  }
};


class Deleter {
  graal_isolatethread_t* thread;
public:
  explicit Deleter(graal_isolatethread_t* thread) : thread(thread) {}

  void operator()(dxfg_event_clazz_list_t* p) const {
    std::cout << "Deleter is deleting a dxfg_event_clazz_list_t" << std::endl;
    dxfg_CList_EventClazz_release(thread, p);
  };
};

void printUsage() {
  std::cout <<
    "Usage:" << std::endl <<
    "DxFeedSample <symbol>" << std::endl << std::endl <<

    "Where:" << std::endl <<
    "  symbol    - Is security symbol (e.g. IBM, AAPL, SPX etc.)." << std::endl;
}

void cPrint(const char* listenerPrefix, const dxfg_event_type_t* pEvent){
  if (pEvent && pEvent->clazz == DXFG_EVENT_QUOTE) {
    auto quote = reinterpret_cast<const dxfg_quote_t*>(pEvent);
    printf(
      "%s: QUOTE{event_symbol=%s, bid_price=%f, bid_time=%lld, ask_price=%f, ask_time=%lld}\n",
      listenerPrefix,
      quote->market_event.event_symbol,
      quote->bid_price,
      quote->bid_time,
      quote->ask_price,
      quote->ask_time);
  }
  if (pEvent && pEvent->clazz == DXFG_EVENT_TRADE) {
    auto trade = reinterpret_cast<const dxfg_trade_t*>(pEvent);
    printf(
      "%s: Trade{event_symbol=%s, price=%f, change=%f, day_volume=%f, day_turnover=%f}\n",
      listenerPrefix,
      trade->trade_base.market_event.event_symbol,
      trade->trade_base.price,
      trade->trade_base.change,
      trade->trade_base.day_volume,
      trade->trade_base.day_turnover);
  }
}

Resources testQuoteListener(graal_isolatethread_t* thread, dxfg_string_symbol_t& symbol) {
  // Create feed and subscription with specified types attached to feed.
  auto feed = dxfg_DXFeed_getInstance(thread);
  dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_QUOTE);

  // Add event listener.
  auto listener = [](graal_isolatethread_t* thread, dxfg_event_type_list* events, void* user_data) {
    for (int i = 0; i < events->size; ++i) {
      cPrint("QuoteListener", events->elements[i]);
    }
  };
  dxfg_feed_event_listener_t* listenerWrapper = dxfg_DXFeedEventListener_new(thread, listener, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscription, listenerWrapper);

  // Add symbol to subscription.
  dxfg_DXFeedSubscription_addSymbol(thread, subscription, &symbol.supper);

  // Save resources that have to be released.
  return Resources { thread, subscription, listenerWrapper };
}

Resources testQuoteAndTradeListener(graal_isolatethread_t* thread, dxfg_string_symbol_t& symbol) {
  // Create feed, list of event types  and subscription with specified types attached to feed.
  auto feed = dxfg_DXFeed_getInstance(thread);
  Deleter deleter(thread);
  auto list = std::unique_ptr<dxfg_event_clazz_list_t, Deleter>(new dxfg_event_clazz_list_t(), deleter);
  list->size = 2;
  list->elements = new dxfg_event_clazz_t*[2];
  list->elements[0] = new dxfg_event_clazz_t(DXFG_EVENT_QUOTE);
  list->elements[1] = new dxfg_event_clazz_t(DXFG_EVENT_TRADE);
  dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription2(thread, feed, list.get());

  // Add event listener.
  auto listener = [](graal_isolatethread_t* thread, dxfg_event_type_list* events, void* user_data) {
    for (int i = 0; i < events->size; ++i) {
      cPrint("QuoteOrTradeListener", events->elements[i]);
    }
  };
  dxfg_feed_event_listener_t* listenerWrapper = dxfg_DXFeedEventListener_new(thread, listener, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscription, listenerWrapper);

  // Add symbol to subscription.
  dxfg_DXFeedSubscription_addSymbol(thread, subscription, &symbol.supper);

  // Save resources that have to be released.
  return Resources { thread, subscription, listenerWrapper };
}
/**
 * Creates multiple event listener and subscribe to Quote and Trade events.
 * Use default DXFeed instance for that data feed address is defined by "dxfeed.properties" file.
 *
 * todo: copy file "dxfeed.properties" to build directory before start the sample
 * */

int main(int argc, char** argv) {
  if (argc < 2) {
    printUsage();
    return -1;
  }

  // Get args from console.
  const auto symbol = argv[1];   // "AAPL";

  // Create STRING symbol.
  dxfg_string_symbol_t stringSymbol;
  stringSymbol.supper.type = STRING;
  stringSymbol.symbol = symbol;

  // Create thread on which all the work will be executed.
  auto thread = create_thread();
  if (thread == nullptr) {
    return -1;
  }

  // Create subscriptions with specified types attached to feed. Close resources automatically on exitscope.
  Resources resources1 = testQuoteListener(thread, stringSymbol);
  Resources resources2 = testQuoteAndTradeListener(thread, stringSymbol);

  // Sleep 10 seconds waiting for the response about Quotes.
  std::chrono::seconds seconds(10);
  std::this_thread::sleep_for(seconds);
}
