// SPDX-License-Identifier: MPL-2.0

#include <cstdint>
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEndpoint::DxEndpoint(JNIEnv* env) :
    env_{env}
  {
    dxEndpointClass_ = jni::safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint;");
    jobject dxEndpointBuilder = createDxEndpointBuilder();
    dxEndpoint_ = env->NewGlobalRef(createDxEndpoint(dxEndpointBuilder));
    env_->DeleteLocalRef(dxEndpointBuilder);
  }

  DxEndpoint::~DxEndpoint() {
    env_->DeleteGlobalRef(dxEndpoint_);
  }

  jobject DxEndpoint::createDxEndpointBuilder() {
    jmethodID newBuilderMethodId = jni::safeGetStaticMethodID(env_, dxEndpointClass_, "newBuilder",
                                                              "()Lcom/dxfeed/api/DXEndpoint$Builder;");
    jobject pJobject = env_->CallStaticObjectMethod(dxEndpointClass_, newBuilderMethodId);
    return pJobject;
  }

  jobject DxEndpoint::createDxEndpoint(jobject dxEndpointBuilder) {
    jclass dxEndpointBuilderClass = env_->GetObjectClass(dxEndpointBuilder);
    jmethodID buildId = jni::safeGetMethodID(env_, dxEndpointBuilderClass, "build", "()Lcom/dxfeed/api/DXEndpoint;");
    env_->DeleteLocalRef(dxEndpointBuilderClass);
    return env_->CallObjectMethod(dxEndpointBuilder, buildId);
  }

  int32_t DxEndpoint::connect(const char* address) {
    jmethodID connectMethodId = jni::safeGetMethodID(env_, dxEndpointClass_, "connect", "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;");
    jstring addr = env_->NewStringUTF(address);
    jobject pJobject = env_->CallObjectMethod(dxEndpoint_, connectMethodId, addr);
    env_->DeleteLocalRef(addr);
    env_->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env_->NewGlobalRef(pJobject);
    return 0;
  }

  DxFeed* DxEndpoint::getFeed() const {
    jmethodID getFeedId = jni::safeGetMethodID(env_, dxEndpointClass_, "getFeed", "()Lcom/dxfeed/api/DXFeed;");
    jobject dxFeed = env_->CallObjectMethod(dxEndpoint_, getFeedId);
    auto* pFeed = new DxFeed(env_, dxFeed);
    env_->DeleteLocalRef(dxFeed);
    return pFeed;
  }

  void DxEndpoint::close() const {
    jmethodID closeMethodId = jni::safeGetMethodID(env_, dxEndpointClass_, "close", "()V");
    env_->CallVoidMethod(dxEndpoint_, closeMethodId);
  }
}