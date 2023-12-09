// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include <chrono>
#include <thread>

#include "api/dxfg_api.h"

void c_print(graal_isolatethread_t *thread, dxfg_event_type_list *events, void *user_data) {
  for (int i = 0; i < events->size; ++i) {
    auto pEvent = events->elements ? events->elements[i] : nullptr;
    if (pEvent && pEvent->clazz == DXFG_EVENT_QUOTE) {
      const auto* quote = (const dxfg_quote_t*) pEvent;
      printf(
        "C: QUOTE{event_symbol=%s, bid_price=%f, bid_time=%lld, ask_price=%f, ask_time=%lld}\n",
        quote->market_event.event_symbol,
        quote->bid_price,
        quote->bid_time,
        quote->ask_price,
        quote->ask_time
            );
    }
    fflush(stdout);
  }
}

void endpoint_state_change_listener(graal_isolatethread_t *thread, dxfg_endpoint_state_t old_state,
                                    dxfg_endpoint_state_t new_state, void *user_data) {
  printf("C: state %d -> %d\n", old_state, new_state);
}

void dxEndpointSubscription(graal_isolatethread_t *thread, const char* address, const char* symbol) {
  dxfg_endpoint_t* endpoint = dxfg_DXEndpoint_create(thread);
  dxfg_DXEndpoint_connect(thread, endpoint, address);
  dxfg_feed_t* feed = dxfg_DXEndpoint_getFeed(thread, endpoint);

  dxfg_subscription_t* subscription = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_QUOTE);
  dxfg_feed_event_listener_t* listener = dxfg_DXFeedEventListener_new(thread, &c_print, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscription, listener);

  dxfg_endpoint_state_change_listener_t* stateListener =
    dxfg_PropertyChangeListener_new(thread, endpoint_state_change_listener, nullptr);
  dxfg_DXEndpoint_addStateChangeListener(thread, endpoint, stateListener);

  dxfg_string_symbol_t symbolAAPL;
  symbolAAPL.supper.type = STRING;
  symbolAAPL.symbol = symbol;

  dxfg_DXFeedSubscription_setSymbol(thread, subscription, &symbolAAPL.supper);
  std::chrono::seconds seconds(10);
  std::this_thread::sleep_for(seconds);

  dxfg_DXFeedSubscription_close(thread, subscription);
  dxfg_DXEndpoint_removeStateChangeListener(thread, endpoint, stateListener);
  dxfg_DXEndpoint_close(thread, endpoint);
  dxfg_JavaObjectHandler_release(thread, &stateListener->handler);
  dxfg_JavaObjectHandler_release(thread, &listener->handler);
  dxfg_JavaObjectHandler_release(thread, &subscription->handler);
  dxfg_JavaObjectHandler_release(thread, &endpoint->handler);
}

int main(int argc, char** argv) {
  // load cmd args
  const int defaultArgSize = 3;
  if (argc < defaultArgSize) {
    std::cerr << "Error: expected 2 args: <address:port, symbol>" << std::endl;
    return -1;
  }

  const auto address = argv[1]; // "demo.dxfeed.com:7300";
  const auto symbol = argv[2];  // "APPL";

  auto thread = create_thread();
  if (thread != nullptr) {
    dxEndpointSubscription(thread, address, symbol);
  }
}