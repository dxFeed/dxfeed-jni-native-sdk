// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed::jni {
  void initDxJni(JNIEnv* env) {
    javaLogger->trace(env, "com.dxfeed.api.DxFeedJniClass: %", safeFindClass(env, "com/dxfeed/api/DxFeedJni"));
    javaLogger->trace(env, "com.dxfeed.api.DxSymbolJniClass: %", safeFindClass(env, "com/dxfeed/api/DxSymbolJni"));

    auto dxEndpointJniClass = safeFindClass(env, "com/dxfeed/api/DxEndpointJni");
    javaLogger->trace(env, "com.dxfeed.api.DxEndpointJniClass: %", dxEndpointJniClass);
    jint nEndpointLoaded = env->RegisterNatives(dxEndpointJniClass, &nDxEndpoint, 1);
    env->DeleteLocalRef(dxEndpointJniClass);

    auto dxSubscriptionJniClass = safeFindClass(env, "com/dxfeed/api/DxSubscriptionJni");
    javaLogger->trace(env, "com.dxfeed.api.DxSubscriptionJniClass: %", dxSubscriptionJniClass);
    jint nSubscriptionLoaded = env->RegisterNatives(dxSubscriptionJniClass, &nDxSubscription, 1);
    env->DeleteLocalRef(dxSubscriptionJniClass);

    auto msg = (nEndpointLoaded == JNI_OK && nSubscriptionLoaded == JNI_OK) ? "JNI_OK" : "Failed";
    javaLogger->trace(env, "RegisterNatives result: %", msg);
  }
}
