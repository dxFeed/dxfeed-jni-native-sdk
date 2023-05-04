// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEndpoint::DxEndpoint(JNIEnv* env, jobject dxEndpoint) :
    env_{env}
  {
    dxEndpointClass_ = env->GetObjectClass(dxEndpoint);
    dxEndpoint_ = env->NewGlobalRef(dxEndpoint);
  }

  DxEndpoint::~DxEndpoint() {
    env_->DeleteGlobalRef(dxEndpoint_);
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

  void DxEndpoint::awaitNotConnected() const {
    jmethodID closeMethodId = jni::safeGetMethodID(env_, dxEndpointClass_, "awaitNotConnected", "()V");
    env_->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

}