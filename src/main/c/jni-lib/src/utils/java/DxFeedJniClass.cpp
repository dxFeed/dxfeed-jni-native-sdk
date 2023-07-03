// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/java/JavaLogger.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed::jni {
  DxJni::DxJni(JNIEnv* env):
    dxEndpointJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxEndpointJni;")))),
    dxFeedJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;")))),
    dxSubscriptionJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxSubscriptionJni;")))),
    dxSymbolJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxSymbolJni;"))))
  {
    javaLogger->info("com.dxfeed.api.DxEndpointJniClass: %", dxEndpointJniClass_);
    javaLogger->info("com.dxfeed.api.DxFeedJniClass: %", dxFeedJniClass_);
    javaLogger->info("com.dxfeed.api.DxSubscriptionJniClass: %", dxSubscriptionJniClass_);
    javaLogger->info("com.dxfeed.api.DxSymbolJniClass: %", dxSymbolJniClass_);
  }

  DxJni* DxJni::initDxJni(JNIEnv* env) {
    auto dxJni = new DxJni(env);
    jint nEndpointLoaded = env->RegisterNatives(dxJni->dxEndpointJniClass_, nDxEndpoint, 1);
    jint nSubscriptionLoaded = env->RegisterNatives(dxJni->dxSubscriptionJniClass_, nDxSubscription, 1);
    auto msg = (nEndpointLoaded == JNI_OK && nSubscriptionLoaded == JNI_OK) ? "JNI_OK" : "Failed";
    javaLogger->info("RegisterNatives result: %", msg);
    return dxJni;
  }
}
