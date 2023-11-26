// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include <chrono>
#include <thread>

#include "api/dxfg_api.h"
#include "api/dxfg_catch_exception.h"
#include "dxfeed/utils/JNICommon.hpp"

void printEvent(const dxfg_event_type_t* pEvent) {
  if (pEvent->clazz == DXFG_EVENT_QUOTE) {
    const auto* quote = (const dxfg_quote_t*) pEvent;
    printf(
      "C: QUOTE{event_symbol=%s, bid_price=%f, bid_time=%lld, ask_price=%f, ask_time=%lld}\n",
      quote->market_event.event_symbol,
      quote->bid_price,
      quote->bid_time,
      quote->ask_price,
      quote->ask_time
    );
  } else {
    printf(
      "C: %u{}\n",
      pEvent->clazz
    );
  }
  fflush(stdout);
}

void c_print(graal_isolatethread_t *thread, dxfg_event_type_list *events, void *user_data) {
  for (int i = 0; i < events->size; ++i) {
    printEvent(events->elements[i]);
  }
}

void endpoint_state_change_listener(graal_isolatethread_t *thread, dxfg_endpoint_state_t old_state,
                                    dxfg_endpoint_state_t new_state, void *user_data) {
  printf("C: state %d -> %d\n", old_state, new_state);
}

void dxEndpointSubscription(graal_isolatethread_t *thread, const char* address, const char* symbol) {
  printf("C: dxEndpointSubscription BEGIN\n");
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

  auto event = dxfg_EventType_new(thread, "", DXFG_EVENT_QUOTE);
  int32_t result = dxfg_DXFeed_getLastEvent(thread, feed, event);
  printf("C: result: %d\n", result);
  dxfg_DXFeed_getLastEventIfSubscribed(thread, feed, DXFG_EVENT_QUOTE, &symbolAAPL.supper);

  dxfg_DXFeedSubscription_close(thread, subscription);
  dxfg_DXEndpoint_removeStateChangeListener(thread, endpoint, stateListener);
  dxfg_DXEndpoint_close(thread, endpoint);
  dxfg_JavaObjectHandler_release(thread, &stateListener->handler);
  dxfg_JavaObjectHandler_release(thread, &listener->handler);
  dxfg_JavaObjectHandler_release(thread, &subscription->handler);
  dxfg_JavaObjectHandler_release(thread, &endpoint->handler);
  printf("C: dxEndpointSubscription END\n");
}

void publishEvents(graal_isolatethread_t* thread, dxfg_event_type_list *events, void *user_data) {
  auto* outputEndpoint = reinterpret_cast<dxfg_endpoint_t*>(user_data);
  auto* pPublisher = dxfg_DXEndpoint_getPublisher(thread, outputEndpoint);
  std::cout << "pPublisher: " << std::hex << pPublisher << std::endl;
  dxfg_DXPublisher_publishEvents(thread, pPublisher, events);
}

void tapeFile(graal_isolatethread_t *thread) {
  // Determine input and output tapes and specify appropriate configuration parameters
  std::string inputAddress = "file:ConvertTapeFile.in[readAs=stream_data,speed=max]";
  std::string outputAddress = "tape:ConvertTapeFile.out[saveAs=stream_data,format=text]";

  // Create input endpoint configured for tape reading
  auto* inputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  dxfg_DXEndpoint_Builder_withRole(thread, inputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_FEED);
  dxfg_DXEndpoint_Builder_withProperty(thread, inputEndpointBuilder, "dxfeed.wildcard.enable", "true");
  dxfg_DXEndpoint_Builder_withProperty(thread, inputEndpointBuilder, "dxendpoint.eventTime", "true");
  auto inputEndpoint = dxfg_DXEndpoint_Builder_build(thread, inputEndpointBuilder);

  // Create output endpoint configured for tape writing
  auto* outputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  dxfg_DXEndpoint_Builder_withRole(thread, outputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_PUBLISHER);
  dxfg_DXEndpoint_Builder_withProperty(thread, outputEndpointBuilder, "dxfeed.wildcard.enable", "true");
  dxfg_DXEndpoint_Builder_withProperty(thread, outputEndpointBuilder, "dxendpoint.eventTime", "true");
  dxfg_endpoint_t* outputEndpoint = dxfg_DXEndpoint_Builder_build(thread, outputEndpointBuilder);

  auto* pList = dxfg_DXEndpoint_getEventTypes(thread, inputEndpoint);
  auto* pFeed = dxfg_DXEndpoint_getFeed(thread, inputEndpoint);
  auto* pSubscription = dxfg_DXFeed_createSubscription2(thread, pFeed, pList);

  auto* listener = dxfg_DXFeedEventListener_new(thread, &publishEvents, outputEndpoint);
  dxfg_DXFeedSubscription_addEventListener(thread, pSubscription, listener);

  dxfg_wildcard_symbol_t symbolWildcard;
  symbolWildcard.supper.type = WILDCARD;
  dxfg_DXFeedSubscription_addSymbol(thread, pSubscription, reinterpret_cast<dxfg_symbol_t*>(&symbolWildcard));

  dxfg_DXEndpoint_connect(thread, outputEndpoint, outputAddress.c_str());
  dxfg_DXEndpoint_connect(thread, inputEndpoint, inputAddress.c_str());

  dxfg_DXEndpoint_awaitNotConnected(thread, inputEndpoint);
  dxfg_DXEndpoint_closeAndAwaitTermination(thread, inputEndpoint);

  dxfg_DXEndpoint_awaitProcessed(thread, outputEndpoint);
  dxfg_DXEndpoint_closeAndAwaitTermination(thread, outputEndpoint);
}

int main(int argc, char** argv) {
  // load cmd args
  const int defaultArgSize = 4;
  if (argc < defaultArgSize) {
    std::cerr << "Error: expected 3 args: <JAVA_HOME, address:port, symbol, ... VMOptions >" << std::endl;
    return -1;
  }
  const auto javaHomePath = argv[1];
  const int vmOptionsSize = 1;
  const auto address = argv[2];
  const auto symbol = argv[3];
  // const char* jvmArgs = argv[4]; // jvmArgs[0]
  const char* jvmArgs[vmOptionsSize] = { "-Xmx12G" };

  dxfeed::jni::VMOptions vmOptions { javaHomePath, jvmArgs, vmOptionsSize };
  graal_isolate_t* isolate;
  graal_isolatethread_t* thread;
  int hr = graal_create_isolate(&vmOptions, &isolate, &thread);
  if (hr == JNI_OK) {
    dxEndpointSubscription(thread, address, symbol);
  }
}