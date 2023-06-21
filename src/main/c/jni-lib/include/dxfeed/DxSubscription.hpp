// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

#include <jni.h>
#include <string>

#include "api/dxfg_events.h"

namespace dxfeed {
  struct DxEventListener;

  struct DxSubscription final {
    constexpr static const char JAVA_CLASS_NAME[] = "com.dxfeed.api.DXFeedSubscription";

    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType);
    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_list_t* eventClazzes);
    ~DxSubscription();

    DxSubscription(const DxSubscription& other) = delete;
    DxSubscription(DxSubscription&& other) = delete;
    DxSubscription& operator=(const DxSubscription& other) = delete;
    DxSubscription& operator=(DxSubscription&& other) = delete;

    void addListener(JNIEnv* env, DxEventListener* listener) const;
    void removeListener(JNIEnv* env, DxEventListener* listener) const;
    int32_t addSymbol(JNIEnv* env, dxfg_symbol_t* symbol) const;
    int32_t addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) const;
    int32_t setSymbol(JNIEnv* env, dxfg_symbol_t* symbol) const;
    void close(JNIEnv* env) const;

  private:
    jobject subscription_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_