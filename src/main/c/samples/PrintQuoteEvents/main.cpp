// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include "api/dxfg_api.h"

void printUsage() {
  std::cout <<
    "Usage:" << std::endl <<
    "PrintQuoteEvents <symbol>" << std::endl << std::endl <<

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
}

/**
 * A simple sample that shows how to subscribe to quotes for one instruments,
 * and print all received quotes to the console.
 * Use default DXFeed instance for that data feed address is defined by "dxfeed.properties" file.
 * The properties file is copied to the build output directory from the project directory.
 *
 * todo: copy file "dxfeed.properties" to build directory before start the sample
 * */
int main(int argc, char** argv) {
  if (argc < 2) {
    printUsage();
    return -1;
  }

 //  Get args from console.
  const auto symbol = argv[1];   // "AAPL";

  // Create thread on which all the work will be executed.
  auto thread = create_thread();
  if (thread == nullptr) {
    return -1;
  }

  // Create feed and subscription with specified types attached to feed.
  dxfg_feed_t* feed = dxfg_DXFeed_getInstance(thread);
  dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_QUOTE);

  auto listener = [](graal_isolatethread_t* thread, dxfg_event_type_list* events, void* user_data) {
    for (int i = 0; i < events->size; ++i) {
      cPrint("QuoteListener", events->elements[i]);
    }
  };

  // Adds event listener.
  dxfg_feed_event_listener_t* listenerWrapper = dxfg_DXFeedEventListener_new(thread, listener, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscription, listenerWrapper);

  // Create STRING symbol.
  dxfg_string_symbol_t stringSymbol;
  stringSymbol.supper.type = STRING;
  stringSymbol.symbol = symbol;

  // Add symbol to subscription.
  dxfg_DXFeedSubscription_addSymbol(thread, subscription, &stringSymbol.supper);

  // Sleep 10 seconds waiting for the response about Quotes.
  std::chrono::seconds seconds(10);
  std::this_thread::sleep_for(seconds);

  // Close subscription and clear resources.
  dxfg_DXFeedSubscription_close(thread, subscription);
  dxfg_JavaObjectHandler_release(thread, &listenerWrapper->handler);
  dxfg_JavaObjectHandler_release(thread, &subscription->handler);
}
