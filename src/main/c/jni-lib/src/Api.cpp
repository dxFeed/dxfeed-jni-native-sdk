// SPDX-License-Identifier: MPL-2.0

#include "api/Api.h"
#include "api/dxfg_api.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DXEndpoint.hpp"
#include "dxfeed/utils/LoadLibrary.hpp"

void dxfg_init(const char* javaHome, const char** vmArgs, const int vmArgsCount) {
  dxfeed::internal::loadJavaVM(javaHome, vmArgs, vmArgsCount);
}

void* dxfg_get_instance() {
  auto& feed = dxfeed::DxFeed::getInstance();
  return &feed;
}

void* dxfg_create_connection(void* feed, const char* address) {
//  return reinterpret_cast<dxfeed::DxFeed*>(feed)->createConnection(address);
  return nullptr;
}

void* dxfg_create_subscription(void* connection, int eventType) {
//  return reinterpret_cast<dxfeed::Connection*>(connection)->createSubscription(static_cast<EventType>(eventType));
  return nullptr;
}

void dxfg_add_symbol(void *subscription, const char *symbol) {
//  reinterpret_cast<dxfeed::Subscription *>(subscription)->addSymbol(symbol);
}

void dxfg_add_listener(void *subscription, dx_feed_listener listener) {
//  reinterpret_cast<dxfeed::Subscription *>(subscription)->addListener(reinterpret_cast<void (*)(const void *,
//                                                                                                size_t)>(listener));
}


// todo: move to another CPP
dxfg_endpoint_t* dxfg_DXEndpoint_create(graal_isolatethread_t* thread) {
  return reinterpret_cast<dxfg_endpoint_t*>(new dxfeed::DxEndpoint(dxfeed::jniEnv, dxfeed::onClose));
}

int32_t dxfg_DXEndpoint_connect(graal_isolatethread_t *thread, dxfg_endpoint_t* endpoint, const char *address) {
  return reinterpret_cast<dxfeed::DxEndpoint*>(endpoint)->connect(address);
}

dxfg_feed_t* dxfg_DXEndpoint_getFeed(graal_isolatethread_t *thread, dxfg_endpoint_t* endpoint) {
  dxfeed::DxEndpoint* pEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  return reinterpret_cast<dxfg_feed_t*>((pEndpoint->getFeed(endpoint->dxfg_java_object_handle)));
}
