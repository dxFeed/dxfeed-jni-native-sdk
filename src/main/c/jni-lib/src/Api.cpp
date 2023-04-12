// SPDX-License-Identifier: MPL-2.0

#include "api/Api.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/Connection.hpp"

void dxfg_init(const char* javaHome, const char** vmArgs, const int vmArgsCount) {
  dxfeed::DxFeed::initJavaVM(javaHome, vmArgs, vmArgsCount);
}

void* dxfg_get_instance() {
  auto& feed = dxfeed::DxFeed::getInstance();
  return &feed;
}

void* dxfg_create_connection(void* feed, const char* address) {
  return reinterpret_cast<dxfeed::DxFeed*>(feed)->createConnection(address);
}

void* dxfg_create_subscription(void* connection, int eventType) {
  return reinterpret_cast<dxfeed::Connection*>(connection)->createSubscription(static_cast<EventType>(eventType));
}

void dxfg_add_symbol(void *subscription, const char *symbol) {
  reinterpret_cast<dxfeed::Subscription *>(subscription)->addSymbol(symbol);
}

void dxfg_add_listener(void *subscription, dx_feed_listener listener) {
  reinterpret_cast<dxfeed::Subscription *>(subscription)->addListener(reinterpret_cast<void (*)(const void *,
                                                                                                size_t)>(listener));
}

void dxfg_add_diagnostic_listener(void *subscription, int64_t diagnostic_listener) {
  reinterpret_cast<dxfeed::Subscription *>(subscription)->addDiagnosticListener(diagnostic_listener);
}
