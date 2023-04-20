// SPDX-License-Identifier: MPL-2.0

#include "api/Api.h"
#include "api/dxfg_api.h"
#include "dxfeed/DxFeedContext.hpp"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/utils/LoadLibrary.hpp"

void dxfg_init(const char* javaHome, const char** vmArgs, const int vmArgsCount) {
  dxfeed::internal::loadJavaVM(javaHome, vmArgs, vmArgsCount);
}

void* dxfg_get_instance() {
  auto& dxfgContext = dxfeed::DxfgContext::getInstance();
  return &dxfgContext;
}

void dxfg_add_symbol(void *subscription, const char *symbol) {
//  reinterpret_cast<dxfeed::Subscription *>(subscription)->addSymbol(symbol);
}

void dxfg_add_listener(void *subscription, dx_feed_listener listener) {
//  reinterpret_cast<dxfeed::Subscription *>(subscription)->addListener(reinterpret_cast<void (*)(const void *,
//                                                                                                size_t)>(listener));
}

// todo: move to another CPP
dxfg_endpoint_t* dxfg_DXEndpoint_create(graal_isolatethread_t*) {
  return reinterpret_cast<dxfg_endpoint_t*>(new dxfeed::DxEndpoint(dxfeed::jniEnv, dxfeed::onClose));
}

int32_t dxfg_DXEndpoint_release(graal_isolatethread_t*, dxfg_endpoint_t* endpoint) {
  auto* pEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  delete pEndpoint;
  return 0;
}

int32_t dxfg_DXEndpoint_connect(graal_isolatethread_t*, dxfg_endpoint_t* endpoint, const char *address) {
  return reinterpret_cast<dxfeed::DxEndpoint*>(endpoint)->connect(address);
}

dxfg_feed_t* dxfg_DXEndpoint_getFeed(graal_isolatethread_t*, dxfg_endpoint_t* endpoint) {
  auto* pEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  return reinterpret_cast<dxfg_feed_t*>((pEndpoint->getFeed()));
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription(graal_isolatethread_t*,
                                                    dxfg_feed_t* feed, dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(eventClazz));
}

