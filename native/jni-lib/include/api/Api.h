#pragma once
#include <cstdint>

extern "C" {

typedef void(*dx_feed_listener)(const void *events, int count);

void dxfeed_init(const char* javaHome, char** argv, int vmArgsCount);
void* dxfeed_get_instance();
void* dxfeed_create_connection(void* feed, const char* address);
void* dxfeed_create_subscription(void* connection, int eventType);
void dxfeed_add_symbol(void *subscription, const char *symbol);
void dxfeed_add_listener(void *subscription, dx_feed_listener listener);
void dxfeed_add_diagnostic_listener(void *subscription, int64_t diagnostic_listener);

}