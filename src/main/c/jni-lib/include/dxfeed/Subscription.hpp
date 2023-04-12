// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

#include <jni.h>
#include <string>
#include <vector>

#include "DxFeed.hpp"
#include "api/EventTypes.h"
#include "dxfeed/utils/Diagnostic.hpp"
#include "dxfeed/utils/TimeAndSaleFormatter.hpp"

namespace dxfeed {
  struct Subscription final {
    typedef void(Listener)(const void* events, std::size_t count);

    Subscription(JNIEnv* env, jobject connection, EventType eventType, dxfeed::OnCloseHandler onClose);
    Subscription(const Subscription& other) = delete;
    Subscription(Subscription&& other) = delete;
    Subscription& operator=(const Subscription& other) = delete;
    Subscription& operator=(Subscription&& other) = delete;
    ~Subscription();

    void addListener(Listener listener) const;
    void addSymbol(const std::string& symbol) const;
//    void addSymbols(const std::vector <std::string>& symbols) const;


    // diagnostic
    void addDiagnosticListener(int64_t listener) const;
  private:
    jobject subscription_;
    JNIEnv* env_;
    const dxfeed::OnCloseHandler onClose_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_