// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

#include <jni.h>
#include <string>
#include <vector>

#include "api/dxfg_events.h"
#include "api/dxfg_subscription.h"

namespace dxfeed {
  struct DxSubscription final {
    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType);
    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_list_t* eventClazzes);
    ~DxSubscription();

    DxSubscription(const DxSubscription& other) = delete;
    DxSubscription(DxSubscription&& other) = delete;
    DxSubscription& operator=(const DxSubscription& other) = delete;
    DxSubscription& operator=(DxSubscription&& other) = delete;

    void addListener(JNIEnv* env, dxfg_feed_event_listener_t* listener) const;
    void addSymbol(JNIEnv* env, const std::string& symbol) const;
    void setSymbol(JNIEnv* env, const std::string& symbol) const;
    void close(JNIEnv* env) const;

  private:
    jobject subscription_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_