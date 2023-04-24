// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_api.h"
#include "dxfeed/DxContext.hpp"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEventListener.hpp"
#include "dxfeed/utils/LoadLibrary.hpp"

void dxfg_init(const char* javaHome, const char** vmArgs, const int vmArgsCount) {
  dxfeed::internal::loadJavaVM(javaHome, vmArgs, vmArgsCount);
}

void* dxfg_get_instance() {
  auto& dxfgContext = dxfeed::DxContext::getInstance();
  return &dxfgContext;
}

// todo: move to another CPP
dxfg_endpoint_t* dxfg_DXEndpoint_create(graal_isolatethread_t*) {
  return reinterpret_cast<dxfg_endpoint_t*>(new dxfeed::DxEndpoint(dxfeed::jniEnv, dxfeed::onClose));
}

int32_t dxfg_DXEndpoint_release(graal_isolatethread_t*, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  delete pDxEndpoint;
  return 0;
}

int32_t dxfg_DXEndpoint_connect(graal_isolatethread_t*, dxfg_endpoint_t* endpoint, const char *address) {
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

dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t*,
                                                         dxfg_feed_event_listener_function user_func, void *user_data)
{
  return reinterpret_cast<dxfg_feed_event_listener_t*>(new dxfeed::DxEventListener(user_func, user_data));
}

int32_t dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t*,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  pDxSubscription->addListener(listener);
  return  0;
}

int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t*, dxfg_subscription_t* sub, dxfg_symbol_t* symbol) {
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
  return  0;
}


