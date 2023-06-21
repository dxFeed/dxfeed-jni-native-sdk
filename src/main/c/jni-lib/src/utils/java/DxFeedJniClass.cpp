// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  DxJni::DxJni(JNIEnv* env):
    dxEndpointJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxEndpointJni;")))),
    dxFeedJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;")))),
    dxSubscriptionJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxSubscriptionJni;")))),
    dxSymbolJniClass_(r_cast<jclass>(env->NewGlobalRef(safeFindClass(env, "Lcom/dxfeed/api/DxSymbolJni;"))))
  {
    std::cout << "com.dxfeed.api.DxEndpointJniClass: " << dxEndpointJniClass_ << "\n";
    std::cout << "com.dxfeed.api.DxFeedJniClass: " << dxFeedJniClass_ << "\n";
    std::cout << "com.dxfeed.api.DxSubscriptionJniClass: " << dxSubscriptionJniClass_ << "\n";
    std::cout << "com.dxfeed.api.DxSymbolJniClass: " << dxSymbolJniClass_ << "\n";
  }

  DxJni* DxJni::initDxJni(JNIEnv* env) {
    auto dxJni = new DxJni(env);
    jint nEndpointLoaded = env->RegisterNatives(dxJni->dxEndpointJniClass_, nDxEndpoint, 1);
    jint nSubscriptionLoaded = env->RegisterNatives(dxJni->dxSubscriptionJniClass_, nDxSubscription, 1);
    auto msg = (nEndpointLoaded == JNI_OK && nSubscriptionLoaded == JNI_OK) ? "JNI_OK" : "Failed";
    std::cout << "\tRegisterNatives result: " << msg << std::endl;
    return dxJni;
  }
}
