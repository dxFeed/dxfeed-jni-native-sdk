// SPDX-License-Identifier: MPL-2.0

#include <memory>

#include "api/dxfg_api.h"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/DxEventListener.hpp"

dxfg_endpoint_builder_t* dxfg_DXEndpoint_newBuilder(graal_isolatethread_t* thread) {
  return reinterpret_cast <dxfg_endpoint_builder_t*>(new dxfeed::DxEndpointBuilder(thread));
}

dxfg_endpoint_t* dxfg_DXEndpoint_Builder_build(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder) {
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  return reinterpret_cast <dxfg_endpoint_t*>(pDxEndpointBuilder->build(thread));
}

// todo: move to another CPP
dxfg_endpoint_t* dxfg_DXEndpoint_create(graal_isolatethread_t* thread) {
  auto pBuilder = std::make_unique<dxfeed::DxEndpointBuilder>(thread);
  dxfeed::DxEndpoint* pEndpoint = pBuilder->build(thread);
  return reinterpret_cast<dxfg_endpoint_t*>(pEndpoint);
}

int32_t dxfg_DXEndpoint_release(graal_isolatethread_t*, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  delete pDxEndpoint;
  return 0;
}

int32_t dxfg_DXEndpoint_close(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  pDxEndpoint->close();
  return 0;
}

int32_t dxfg_DXEndpoint_connect(graal_isolatethread_t*, dxfg_endpoint_t* endpoint, const char* address) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  return pDxEndpoint->connect(address);
}

dxfg_feed_t* dxfg_DXEndpoint_getFeed(graal_isolatethread_t*, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  return reinterpret_cast<dxfg_feed_t*>(pDxEndpoint->getFeed());
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription(graal_isolatethread_t*,
                                                    dxfg_feed_t* feed, dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(eventClazz));
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription2(graal_isolatethread_t* thread,
                                                     dxfg_feed_t* feed, dxfg_event_clazz_list_t* eventClazzes)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(eventClazzes));
}


int32_t dxfg_DXSubscription_release(graal_isolatethread_t*, dxfg_subscription_t* subscription) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(subscription);
  delete pDxSubscription;
  return 0;
}

int32_t dxfg_DXFeedSubscription_close(graal_isolatethread_t*, dxfg_subscription_t* sub) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  pDxSubscription->close();
  return 0;
}

dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t*,
                                                         dxfg_feed_event_listener_function user_func, void* user_data)
{
  return reinterpret_cast<dxfg_feed_event_listener_t*>(new dxfeed::DxEventListener(user_func, user_data));
}

int32_t dxfg_DXFeedEventListener_release(graal_isolatethread_t*, dxfg_feed_event_listener_t* listener) {
  auto* pListener = reinterpret_cast<dxfeed::DxEventListener*>(listener);
  delete pListener;
  return 0;
}

int32_t dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t*,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  pDxSubscription->addListener(listener);
  return 0;
}

int32_t dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t*, dxfg_subscription_t* sub, dxfg_symbol_t* symbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (symbol->type) {
    case STRING: {
      auto* pSymbol = reinterpret_cast<dxfg_string_symbol_t*>(symbol);
      // todo: investigate, why there no setSymbols method. Inlined?
      //  for now use addSymbol instead of setSymbol
      pDxSubscription->addSymbol(pSymbol->symbol);
      break;
    }
    default:
      return -1;
  }
  return 0;
}

int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t*, dxfg_subscription_t* sub, dxfg_symbol_t* symbol) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (symbol->type) {
    case STRING: {
      auto* pSymbol = reinterpret_cast<dxfg_string_symbol_t*>(symbol);
      pDxSubscription->addSymbol(pSymbol->symbol);
      break;
    }
    default:
      return -1;
  }
  return 0;
}

int dxfg_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler*) {
  return 0; // todo: think
}



