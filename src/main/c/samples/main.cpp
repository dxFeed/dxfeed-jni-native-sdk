// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include <chrono>
#include <thread>

#include "api/dxfg_api.h"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

void printEvent(const dxfg_event_type_t* pEvent) {
  if (pEvent->clazz == DXFG_EVENT_TIME_AND_SALE) {
    const auto* time_and_sale = (const dxfg_time_and_sale_t*) pEvent;
    printf(
      "C: TIME_AND_SALE{event_symbol=%s, bid_price=%f, exchange_sale_conditions=%s, buyer=%s, seller=%s}\n",
      time_and_sale->market_event.event_symbol,
      time_and_sale->bid_price,
      time_and_sale->exchange_sale_conditions,
      time_and_sale->buyer,
      time_and_sale->seller
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

void finalize(graal_isolatethread_t *thread, void *userData) {
  printf("C: finalize\n");
}

void dxEndpointSubscription(graal_isolatethread_t *thread) {
  printf("C: dxEndpointSubscription BEGIN\n");
  dxfg_endpoint_t* endpoint = dxfg_DXEndpoint_create(thread);
  dxfg_DXEndpoint_connect(thread, endpoint, "demo.dxfeed.com:7300");
  dxfg_feed_t* feed = dxfg_DXEndpoint_getFeed(thread, endpoint);

  dxfg_subscription_t* subscriptionTimeAndSale = dxfg_DXFeed_createSubscription(thread, feed, DXFG_EVENT_TIME_AND_SALE);
  dxfg_feed_event_listener_t* listener = dxfg_DXFeedEventListener_new(thread, &c_print, nullptr);
//  dxfg_Object_finalize(thread, (dxfg_java_object_handler*)listener, finalize, nullptr);
  dxfg_DXFeedSubscription_addEventListener(thread, subscriptionTimeAndSale, listener);

  dxfg_endpoint_state_change_listener_t* stateListener = dxfg_PropertyChangeListener_new(thread, endpoint_state_change_listener, nullptr);
//  dxfg_Object_finalize(thread, stateListener, finalize, nullptr);
  dxfg_DXEndpoint_addStateChangeListener(thread, endpoint, stateListener);

  dxfg_string_symbol_t symbolAAPL;
  symbolAAPL.supper.type = STRING;
  symbolAAPL.symbol = "AAPL";

  dxfg_DXFeedSubscription_setSymbol(thread, subscriptionTimeAndSale, &symbolAAPL.supper);
//  int containQuote = dxfg_DXFeedSubscription_containsEventType(thread, subscriptionTimeAndSale, DXFG_EVENT_TIME_AND_SALE);
//  int containCandle = dxfg_DXFeedSubscription_containsEventType(thread, subscriptionTimeAndSale, DXFG_EVENT_QUOTE);
  std::chrono::seconds minutes(10); // time to sleep 24 hours
  std::this_thread::sleep_for(minutes);

  auto event = dxfg_EventType_new(thread, "", DXFG_EVENT_QUOTE);
  int32_t result = dxfg_DXFeed_getLastEvent(thread, feed, event);
  printf("C: result: %d\n", result);
  dxfg_DXFeed_getLastEventIfSubscribed(thread, feed, DXFG_EVENT_QUOTE, &symbolAAPL.supper);

  dxfg_DXFeedSubscription_close(thread, subscriptionTimeAndSale);
  dxfg_DXEndpoint_removeStateChangeListener(thread, endpoint, stateListener);
  dxfg_DXEndpoint_close(thread, endpoint);
  dxfg_JavaObjectHandler_release(thread, &stateListener->handler);
  dxfg_JavaObjectHandler_release(thread, &listener->handler);
  dxfg_JavaObjectHandler_release(thread, &subscriptionTimeAndSale->handler);
  dxfg_JavaObjectHandler_release(thread, &endpoint->handler);
  printf("C: dxEndpointSubscription END\n");
}

void publishEvents(graal_isolatethread_t* thread, dxfg_event_type_list *events, void *user_data) {
  auto* outputEndpoint = reinterpret_cast<dxfg_endpoint_t*>(user_data);
  auto* pPublisher = dxfg_DXEndpoint_getPublisher(thread, outputEndpoint);
  std::cout << "pPublisher 0x: " << 0 << std::endl;
  dxfg_DXPublisher_publishEvents(thread, pPublisher, events);
}

void tapeFile(graal_isolatethread_t *thread) {
//  auto jDxEndpointClass = dxfeed::jni::safeFindClass(thread, "Lcom/dxfeed/api/DxEndpointJni;");
//  const char* methodName = "tapeFile";
//  const char* methodSignature = "()V";
//  auto methodId = dxfeed::jni::safeGetStaticMethodID(thread, jDxEndpointClass, methodName, methodSignature);
//  thread->CallStaticVoidMethod(jDxEndpointClass, methodId);
//  thread->DeleteLocalRef(jDxEndpointClass);

  // Determine input and output tapes and specify appropriate configuration parameters
  std::string inputAddress = "file:ConvertTapeFile.in[readAs=stream_data,speed=max]";
  std::string outputAddress = "tape:ConvertTapeFile.out[saveAs=stream_data,format=text]";

//  // Create input endpoint configured for tape reading
  auto* inputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  int32_t result = dxfg_DXEndpoint_Builder_withRole(thread, inputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_FEED);
  result = dxfg_DXEndpoint_Builder_withProperty(thread, inputEndpointBuilder, "dxfeed.wildcard.enable", "true");
  result = dxfg_DXEndpoint_Builder_withProperty(thread, inputEndpointBuilder, "dxendpoint.eventTime", "true");
  auto inputEndpoint = dxfg_DXEndpoint_Builder_build(thread, inputEndpointBuilder);

  // Create output endpoint configured for tape writing
  auto* outputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  dxfg_DXEndpoint_Builder_withRole(thread, outputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_PUBLISHER);
  dxfg_DXEndpoint_Builder_withProperty(thread, outputEndpointBuilder, "dxfeed.wildcard.enable", "true");
  dxfg_DXEndpoint_Builder_withProperty(thread, outputEndpointBuilder, "dxendpoint.eventTime", "true");
  dxfg_endpoint_t* outputEndpoint = dxfg_DXEndpoint_Builder_build(thread, outputEndpointBuilder);

  dxfg_event_clazz_list_t* pList = dxfg_DXEndpoint_getEventTypes(thread, inputEndpoint);
  dxfg_feed_t* pFeed = dxfg_DXEndpoint_getFeed(thread, inputEndpoint);
  dxfg_subscription_t* pSubscription = dxfg_DXFeed_createSubscription2(thread, pFeed, pList);

  dxfg_feed_event_listener_t* listener = dxfg_DXFeedEventListener_new(thread, &publishEvents, outputEndpoint);
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
  const int vmOptionsSize = 2;
  const char* jvmArgs[vmOptionsSize] = { "-Xmx12G", "-Dcom.devexperts.qd.impl.matrix.Agent.MaxBufferSize=50000000" };
  const auto address = argv[2];
  const auto symbol = argv[3];

  // init context, connection, subscription
  std::cout << "Connection to address:" << address << std::endl;
  dxfeed::jni::VMOptions vmOptions { javaHomePath, jvmArgs, vmOptionsSize };
  graal_isolate_t* isolate;
  graal_isolatethread_t* thread;
  int hr = graal_create_isolate(&vmOptions, &isolate, &thread);
  if (hr == JNI_OK) {
//    dxEndpointSubscription(thread);
    tapeFile(thread);
  }
}