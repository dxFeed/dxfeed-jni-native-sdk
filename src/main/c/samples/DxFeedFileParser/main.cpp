// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <string>

#include "api/dxfg_endpoint.h"
#include "api/dxfg_feed.h"
#include "api/dxfg_subscription.h"

static int _eventCounter = 0;

void publishEvents(graal_isolatethread_t* thread, dxfg_event_type_list *events, void *user_data) {
  for (int i = 0; i < events->size; ++i) {
    dxfg_event_type_t* pEvent = events->elements[i];
    if (pEvent && pEvent->clazz == DXFG_EVENT_QUOTE) {
      const auto* quote = (const dxfg_quote_t*) pEvent;
      printf(
        "%d: QUOTE{event_symbol=%s, bid_price=%f, bid_time=%lld, ask_price=%f, ask_time=%lld}\n",
        ++_eventCounter,
        quote->market_event.event_symbol,
        quote->bid_price,
        quote->bid_time,
        quote->ask_price,
        quote->ask_time);
    }
  }
}

/**
 * Reads events form a tape file.
 *
 * todo: copy file "ConvertTapeFile.in" to build directory before start the sample
 * */
int main(int argc, char** argv) {
  // Create thread on which all the work will be executed.
  auto thread = create_thread();
  if (thread == nullptr) {
    return -1;
  }

  // Determine input and output tapes and specify appropriate configuration parameters.
  std::string inputFile = argc > 1 ? argv[1] : "ConvertTapeFile.in";
  dxfg_event_clazz_t eventType = DXFG_EVENT_QUOTE;
  std::string symbol = "AAPL";

  // Create input endpoint configured for tape reading.
  auto* inputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  dxfg_DXEndpoint_Builder_withRole(thread, inputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_FEED);
  auto inputEndpoint = dxfg_DXEndpoint_Builder_build(thread, inputEndpointBuilder);

  // Subscribe to a specified event and symbol.
  auto* pFeed = dxfg_DXEndpoint_getFeed(thread, inputEndpoint);
  auto* pSubscription = dxfg_DXFeed_createSubscription(thread, pFeed, eventType);
  auto* listener = dxfg_DXFeedEventListener_new(thread, &publishEvents, inputEndpoint);
  dxfg_DXFeedSubscription_addEventListener(thread, pSubscription, listener);

  // Create STRING symbol.
  dxfg_string_symbol_t stringSymbol;
  stringSymbol.supper.type = STRING;
  stringSymbol.symbol = symbol.c_str();

  // Add symbol to subscription.
  dxfg_DXFeedSubscription_addSymbol(thread, pSubscription, &stringSymbol.supper);

  // Connect endpoint to a file.
  auto argFile = std::string("file:") + inputFile + "[speed=max]";
  dxfg_DXEndpoint_connect(thread, inputEndpoint, argFile.c_str());

  // Wait until file is completely parsed.
  dxfg_DXEndpoint_awaitNotConnected(thread, inputEndpoint);

  // Close endpoint when we're done.
  // This method will gracefully close endpoint, waiting while data processing completes.
  dxfg_DXEndpoint_closeAndAwaitTermination(thread, inputEndpoint);

  // Close subscription and clear resources.
  dxfg_DXFeedSubscription_close(thread, pSubscription);
  dxfg_JavaObjectHandler_release(thread, &pSubscription->handler);
  dxfg_JavaObjectHandler_release(thread, &listener->handler);
  dxfg_JavaObjectHandler_release(thread, &inputEndpointBuilder->handler);
  dxfg_JavaObjectHandler_release(thread, &inputEndpoint->handler);
}