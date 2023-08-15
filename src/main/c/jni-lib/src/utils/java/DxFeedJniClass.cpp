// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/java/JavaLogger.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed::jni {
  void initDxJni(JNIEnv* env) {
    jclass dxEndpointJniClass = safeFindClass(env, "Lcom/dxfeed/api/DxEndpointJni;");
    javaLogger->info("com.dxfeed.api.DxEndpointJniClass: %", dxEndpointJniClass);
    javaLogger->info("com.dxfeed.api.DxFeedJniClass: %", safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;"));
    jclass dxSubscriptionJniClass = safeFindClass(env, "Lcom/dxfeed/api/DxSubscriptionJni;");
    javaLogger->info("com.dxfeed.api.DxSubscriptionJniClass: %", dxSubscriptionJniClass);
    javaLogger->info("com.dxfeed.api.DxSymbolJniClass: %", safeFindClass(env, "Lcom/dxfeed/api/DxSymbolJni;"));
    jint nEndpointLoaded = env->RegisterNatives(dxEndpointJniClass, nDxEndpoint, 1);
    jint nSubscriptionLoaded = env->RegisterNatives(dxSubscriptionJniClass, nDxSubscription, 1);
    auto msg = (nEndpointLoaded == JNI_OK && nSubscriptionLoaded == JNI_OK) ? "JNI_OK" : "Failed";
    javaLogger->info("RegisterNatives result: %", msg);
  }
}
