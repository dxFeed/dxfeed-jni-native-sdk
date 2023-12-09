// SPDX-License-Identifier: MPL-2.0

#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>

#include "api/dxfg_api.h"
#include "dxfeed/utils/JNICommon.hpp"

void printUsage() {
  std::cout <<
    "Usage:" << std::endl <<
    "DxFeedConnect <JAVA_HOME> <address> <types> <symbols>" << std::endl << std::endl <<

    "Where:" << std::endl <<
    "  JAVA_HOME - the absolute path directory where JDK(JRE) is installed," << std::endl <<
    "              e.g.: /Users/FirstName.SecondName/Documents/amazon-corretto-8.jdk/Contents/Home" << std::endl <<
    "  address   - The address to connect to retrieve data (remote host or local tape file)." << std::endl <<
    "              To pass an authorization token, add to the address: \"[login=entitle:<token>]\"," << std::endl <<
    "              e.g.: demo.dxfeed.com:7300[login=entitle:<token>]" << std::endl <<
    "  types     - Is comma-separated list of dxfeed event types ({eventTypeNames})." << std::endl <<
    "  symbol    - Is comma-separated list of symbol names to get events for (e.g. ""IBM,AAPL,MSFT"")." << std::endl <<
    "              for Candle event specify symbol with aggregation like in \"AAPL{{=d}}\"" << std::endl << std::endl <<

    "Examples:" << std::endl <<
    "  DxFeedConnect /Users/userName/Documents/java8/Home demo.dxfeed.com:7300 Quote AAPL" << std::endl <<
    "  DxFeedConnect /Users/userName/Documents/java8/Home demo.dxfeed.com:7300 TimeAndSale MSFT" << std::endl;
}

void cPrint(graal_isolatethread_t *thread, dxfg_event_type_list *events, void *user_data) {
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

/**
 * This sample that shows how to subscribe to events.
 * The sample configures via command line, connect to endpoint, subscribes to events and prints received data.
 */
int main(int argc, char** argv) {
  if (argc < 4) {
    printUsage();
    return -1;
  }

  //  Get args from console.
  const auto javaHome = argv[1]; // "/Users/userName/Documents/java8/Home";
  const auto address = argv[2];  // "demo.dxfeed.com:7300";
  const auto eventType = argv[3];   // "Quote";
  const auto symbol = argv[4];   // "AAPL";
  // const char* jvmArgs = argv[5]; // "-Xmx12G"
  printf("-- Use JAVA_HOME from %s. --\n"
         "-- Connect to address %s. --\n"
         "-- Ignore custom event type: %s, use QUOTE by default. --\n"
         "-- Use symbol %s for subscription.--\n\n", javaHome, address, eventType, symbol);

  // Create thread on which all the work will be executed.
  const int vmOptionsSize = 1;
  const char* vmArgs[vmOptionsSize] = { "-Xmx12G" };
  dxfeed::jni::VMOptions vmOptions { javaHome, vmArgs, 1 };
  auto thread = create_thread_with_VMOptions(&vmOptions);
  if (thread == nullptr) {
    return -1;
  }

  // Create endpoint and connect to specified address.
  dxfg_endpoint_t* endpoint = dxfg_DXEndpoint_create(thread);
  dxfg_DXEndpoint_connect(thread, endpoint, address);

  // Create feed and subscription with specified types attached to feed.
  dxfg_feed_t* feed = dxfg_DXEndpoint_getFeed(thread, endpoint);
  dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_QUOTE);

  // Adds event listener.
  dxfg_feed_event_listener_t* listener = dxfg_DXFeedEventListener_new(thread, &cPrint, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscription, listener);

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
  dxfg_DXEndpoint_close(thread, endpoint);
  dxfg_JavaObjectHandler_release(thread, &listener->handler);
  dxfg_JavaObjectHandler_release(thread, &subscription->handler);
  dxfg_JavaObjectHandler_release(thread, &endpoint->handler);
}