// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_

#include <jni.h>

#include "api/dxfg_subscription.h"

namespace dxfeed {
struct DxSubscriptionChangeListener {
    static DxSubscriptionChangeListener *
    create(JNIEnv *env, dxfg_ObservableSubscriptionChangeListener_function_symbolsAdded fSymbolsAdded,
           dxfg_ObservableSubscriptionChangeListener_function_symbolsRemoved fSymbolsRemoved,
           dxfg_ObservableSubscriptionChangeListener_function_subscriptionClosed fSubscriptionClosed, void *user_data);

  private:
    explicit DxSubscriptionChangeListener(jlong listenerId);
    jobject dummy_ = nullptr;

  public:
    const jlong javaListenerId_;
};
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
