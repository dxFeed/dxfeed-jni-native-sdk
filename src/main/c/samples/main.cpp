// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "api/Api.h"
#include "api/dxfg_api.h"

void printEvent(const dxfg_event_type_t *pEvent) {
  if (pEvent->clazz == DXFG_EVENT_TIME_AND_SALE) {
    auto *time_and_sale = (dxfg_time_and_sale_t *)pEvent;
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
}

void c_print(graal_isolatethread_t *thread, dxfg_event_type_list *events, void *user_data) {
  for (int i = 0; i < events->size; ++i) {
    printEvent(events->elements[i]);
  }
}

void finalize(graal_isolatethread_t *thread, void *userData) {
  printf("C: finalize\n");
}

// todo: implement sample with API
 void dxEndpointTimeSeriesSubscription(graal_isolatethread_t *thread) {
    printf("C: dxEndpointTimeSeriesSubscription BEGIN\n");
    dxfg_endpoint_t* endpoint = dxfg_DXEndpoint_create(thread);
    dxfg_DXEndpoint_connect(thread, endpoint, "demo.dxfeed.com:7300");
    dxfg_feed_t* feed = dxfg_DXEndpoint_getFeed(thread, endpoint);
//    dxfg_event_clazz_list_t* event_clazz_list = dxfg_DXEndpoint_getEventTypes(thread, endpoint);
//    dxfg_time_series_subscription_t* subscriptionTaS = dxfg_DXFeed_createTimeSeriesSubscription2(thread, feed, event_clazz_list);
//    dxfg_CList_EventClazz_release(thread, event_clazz_list);
//    dxfg_DXFeedTimeSeriesSubscription_setFromTime(thread, subscriptionTaS, 0);
//    dxfg_feed_event_listener_t* listener = dxfg_DXFeedEventListener_new(thread, &c_print, nullptr);
//    dxfg_Object_finalize(thread, (dxfg_java_object_handler*)listener, finalize, nullptr);
//    dxfg_DXFeedSubscription_addEventListener(thread, &subscriptionTaS->sub, listener);
//    dxfg_string_symbol_t symbolAAPL;
//    symbolAAPL.supper.type = STRING;
//    symbolAAPL.symbol = "AAPL";
//    dxfg_DXFeedSubscription_setSymbol(thread, &subscriptionTaS->sub, &symbolAAPL.supper);
////    usleep(2000000);
//    dxfg_DXFeedSubscription_close(thread, (dxfg_subscription_t*) subscriptionTaS);
//    dxfg_DXEndpoint_close(thread, endpoint);
//    dxfg_JavaObjectHandler_release(thread, &subscriptionTaS->sub.handler);
//    dxfg_JavaObjectHandler_release(thread, &listener->handler);
//    dxfg_JavaObjectHandler_release(thread, &feed->handler);
//    dxfg_JavaObjectHandler_release(thread, &endpoint->handler);
    printf("C: dxEndpointTimeSeriesSubscription END\n");
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
  const char* jvmArgs[vmOptionsSize] = { "-Xmx12G" };
  const auto address = argv[2];
  const auto symbol = argv[3];

  // init context, connection, subscription
  std::cout << "Connection to address:" << address << std::endl;
  dxfg_init(javaHomePath, jvmArgs, vmOptionsSize);

  dxEndpointTimeSeriesSubscription(nullptr);

//  void* dxFeed = dxfg_get_instance();
//  auto connection = dxfg_create_connection(dxFeed, address);
//  auto subscription = dxfg_create_subscription(connection, 0);
//
//  // add listener with user code
//  dxfg_add_listener(subscription, [](const void *events, int count) {
//    auto timeAndSaleList = reinterpret_cast<const TimeAndSale*>(events);
//    for (int i = 0; i < count; ++i) {
//      auto quote = std::make_shared<TimeAndSale>(timeAndSaleList[i]);
//      std::cout << dxfeed::TimeAndSaleFormatter::toString(quote.get()) << std::endl;
//    }
//  });
//
//  // add symbol to subscription
//  dxfg_add_symbol(subscription, symbol);
//
//    // sleep then clean up
//  std::chrono::hours hours(24); // time to sleep 24 hours
//  std::this_thread::sleep_for(hours);
//  delete subscription;
//  delete connection;
}