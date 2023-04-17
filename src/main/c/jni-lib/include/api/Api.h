// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_API_H_
#define DXFEED_JNI_NATIVE_SDK_API_H_

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*dx_feed_listener)(const void *events, const int count);

void dxfg_init(const char* javaHome, const char** vmArgs, int vmArgsCount);
void* dxfg_get_instance();
void* dxfg_create_connection(void* feed, const char* address);
void* dxfg_create_subscription(void* connection, int eventType);
void dxfg_add_symbol(void *subscription, const char *symbol);
void dxfg_add_listener(void *subscription, dx_feed_listener listener);

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_SDK_API_H_