// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

#include <jni.h>
#include <string>
#include <vector>

#include "DxFeedContext.hpp"
#include "api/EventTypes.h"
#include "api/dxfg_events.h"

namespace dxfeed {
  struct Subscription final {
    typedef void(Listener)(const void* events, std::size_t count);

    Subscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType, dxfeed::OnCloseHandler onClose);
    Subscription(const Subscription& other) = delete;
    Subscription(Subscription&& other) = delete;
    Subscription& operator=(const Subscription& other) = delete;
    Subscription& operator=(Subscription&& other) = delete;
    ~Subscription();

    void addListener(Listener listener) const;
    void addSymbol(const std::string& symbol) const;
//    void addSymbols(const std::vector <std::string>& symbols) const;

  private:
    jobject subscription_;
    JNIEnv* env_;
    const dxfeed::OnCloseHandler onClose_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_