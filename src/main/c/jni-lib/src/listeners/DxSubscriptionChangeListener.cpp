// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/listeners/DxSubscriptionChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxSubscriptionChangeListener::DxSubscriptionChangeListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxSubscriptionChangeListener* DxSubscriptionChangeListener::create(
      JNIEnv* env,
      dxfg_ObservableSubscriptionChangeListener_function_symbolsAdded fSymbolsAdded,
      dxfg_ObservableSubscriptionChangeListener_function_symbolsRemoved fSymbolsRemoved,
      dxfg_ObservableSubscriptionChangeListener_function_subscriptionClosed fSubscriptionClosed,
      void *userData
  ) {
    auto dxEndpointClass = jni::safeFindClass(env, "com/dxfeed/api/DxSubscriptionJni");
    auto newChangeListenerId =
      jni::safeGetStaticMethodID(env, dxEndpointClass, "newChangeListener", "(JJJJ)J");
    auto result = jni::checkedCallStaticLongMethod(env, dxEndpointClass, newChangeListenerId,
                                                   fSymbolsAdded, fSymbolsRemoved, fSubscriptionClosed, userData);
    env->DeleteLocalRef(dxEndpointClass);
    return new DxSubscriptionChangeListener(result);
  }
}