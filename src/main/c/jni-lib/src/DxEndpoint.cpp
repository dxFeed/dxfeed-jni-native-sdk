// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"

namespace dxfeed {
  using namespace jni;

  DxEndpoint::DxEndpoint(JNIEnv* env, jobject dxEndpoint) {
    dxEndpointClass_ = env->GetObjectClass(dxEndpoint);
    dxEndpoint_ = env->NewGlobalRef(dxEndpoint);
  }

  DxEndpoint::~DxEndpoint() {
    internal::jniEnv->DeleteGlobalRef(dxEndpoint_);
  }

  int32_t DxEndpoint::connect(JNIEnv* env, const char* address) {
    jmethodID connectMethodId = safeGetMethodID(env, dxEndpointClass_, "connect", "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;");
    jstring addr = env->NewStringUTF(address);
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpoint_, connectMethodId, addr);
    env->DeleteLocalRef(addr);
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return JNI_OK;
  }

  DxFeed* DxEndpoint::getFeed(JNIEnv* env) const {
    jmethodID getFeedId = safeGetMethodID(env, dxEndpointClass_, "getFeed", "()Lcom/dxfeed/api/DXFeed;");
    jobject dxFeed = env->CallObjectMethod(dxEndpoint_, getFeedId);
    auto* pFeed = new DxFeed(env, dxFeed);
    env->DeleteLocalRef(dxFeed);
    return pFeed;
  }

  void DxEndpoint::close(JNIEnv* env) const {
    jmethodID closeMethodId = safeGetMethodID(env, dxEndpointClass_, "close", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  void DxEndpoint::awaitNotConnected(JNIEnv* env) const {
    jmethodID closeMethodId = safeGetMethodID(env, dxEndpointClass_, "awaitNotConnected", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  void DxEndpoint::addStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) {
    auto dxEndpointClass = internal::dxJni->dxEndpointJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxEndpointClass, "addStateChangeEventListener",
                                               "(Lcom/dxfeed/api/DXEndpoint;J)V");
    env->CallStaticVoidMethod(dxEndpointClass, methodId, dxEndpoint_, listener->javaListenerId_);
  }

  void DxEndpoint::removeStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) {
    auto dxEndpointClass = internal::dxJni->dxEndpointJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxEndpointClass, "removeStateChangeEventListener",
                                                    "(Lcom/dxfeed/api/DXEndpoint;J)V");
    env->CallStaticVoidMethod(dxEndpointClass, methodId, dxEndpoint_, listener->javaListenerId_);
  }

  DxEndpoint* DxEndpoint::getInstance(JNIEnv* env) {
    jclass dxEndpointClass = safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint;");
    jmethodID getInstanceId = safeGetStaticMethodID(env, dxEndpointClass, "getInstance",
                                                    "()Lcom/dxfeed/api/DXEndpoint;");
    jobject dxEndpoint = env->CallStaticObjectMethod(dxEndpointClass, getInstanceId);
    DxEndpoint* result = nullptr;
    if (dxEndpoint) {
      result = new DxEndpoint(env, dxEndpoint);
      env->DeleteLocalRef(dxEndpoint);
    }
    env->DeleteLocalRef(dxEndpointClass);
    return result;
  }
}