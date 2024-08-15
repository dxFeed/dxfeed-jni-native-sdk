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
            "DxFeedSubscriptionChangeListener <symbol>" << std::endl << std::endl <<

            "Where:" << std::endl <<
            "  symbol    - Is security symbol (e.g. IBM, AAPL, SPX etc.)." << std::endl;
}

void cPrint(graal_isolatethread_t *thread, dxfg_event_type_list *events, void *user_data) {
  for (int i = 0; i < events->size; ++i) {
    dxfg_event_type_t *pEvent = events->elements[i];
    if (pEvent && pEvent->clazz == DXFG_EVENT_QUOTE) {
      const auto *quote = (const dxfg_quote_t *) pEvent;
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

void onSymbolAdded(graal_isolatethread_t *thread, dxfg_symbol_list *symbols, void *user_data) {
  if (symbols) {
    int size = symbols->size;
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t *pSymbol = symbols->elements[i];
      printf("Added symbol:\n\ttype=%d\n", pSymbol->type);
      if (pSymbol->type == STRING) {
        printf("\tsymbol=%s\n", reinterpret_cast<dxfg_string_symbol_t *>(pSymbol)->symbol);
      }
    }
  } else {
    printf("No symbols added...\n");
  }
}

void onSymbolRemoved(graal_isolatethread_t *thread, dxfg_symbol_list *symbols, void *user_data) {
  if (symbols) {
    int size = symbols->size;
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t *pSymbol = symbols->elements[i];
      printf("Removed symbol:\n\ttype=%d\n", pSymbol->type);
      if (pSymbol->type == STRING) {
        printf("\tsymbol=%s\n", reinterpret_cast<dxfg_string_symbol_t *>(pSymbol)->symbol);
      }
    }
  } else {
    printf("No symbols removed...\n");
  }
}

void onSubscriptionClosed(graal_isolatethread_t *thread, void *user_data) {
  printf("Subscription closed...\n");
}

/**
 * This sample that shows how to subscribe to events.
 * The sample configures via command line, connect to endpoint, subscribes to events and prints received data.
 */
int main(int argc, char **argv) {
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

  // Create endpoint and connect to specified address.
  dxfg_endpoint_t* endpoint = dxfg_DXEndpoint_create(thread);
  dxfg_DXEndpoint_connect(thread, endpoint, "demo.dxfeed.com:7300");

  // Create feed and subscription with specified types attached to feed.
  auto feed = dxfg_DXEndpoint_getFeed(thread, endpoint);
  dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_QUOTE);

  // Adds event listener.
  dxfg_feed_event_listener_t* eventListener = dxfg_DXFeedEventListener_new(thread, &cPrint, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscription, eventListener);

  // Adds subscription change listener.
  auto changeListener = dxfg_ObservableSubscriptionChangeListener_new(
      thread, onSymbolAdded, onSymbolRemoved, onSubscriptionClosed, nullptr);
  dxfg_DXFeedSubscription_addChangeListener(thread, subscription, changeListener);

  // Add symbol to subscription.
  dxfg_DXFeedSubscription_addSymbol(thread, subscription, &stringSymbol.supper);

  // Sleep 10 seconds waiting for the response about Quotes.
  std::chrono::seconds seconds10(10);
  std::this_thread::sleep_for(seconds10);
  dxfg_DXFeedSubscription_removeSymbol(thread, subscription, &stringSymbol.supper);
  std::chrono::seconds seconds2(2);
  std::this_thread::sleep_for(seconds2);

  // Close subscription and clear resources.
  dxfg_DXFeedSubscription_close(thread, subscription);
  dxfg_JavaObjectHandler_release(thread, &eventListener->handler);
  dxfg_JavaObjectHandler_release(thread, &changeListener->handler);
  dxfg_JavaObjectHandler_release(thread, &subscription->handler);
}