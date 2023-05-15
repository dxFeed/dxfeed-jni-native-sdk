// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEndpoint::DxEndpoint(JNIEnv* env, jobject dxEndpoint) {
    dxEndpointClass_ = env->GetObjectClass(dxEndpoint);
    dxEndpoint_ = env->NewGlobalRef(dxEndpoint);
  }

  DxEndpoint::~DxEndpoint() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(dxEndpoint_);
  }

  int32_t DxEndpoint::connect(JNIEnv* env, const char* address) {
    jmethodID connectMethodId = jni::safeGetMethodID(env, dxEndpointClass_, "connect", "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;");
    jstring addr = env->NewStringUTF(address);
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpoint_, connectMethodId, addr);
    env->DeleteLocalRef(addr);
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return JNI_OK;
  }

  DxFeed* DxEndpoint::getFeed(JNIEnv* env) const {
    jmethodID getFeedId = jni::safeGetMethodID(env, dxEndpointClass_, "getFeed", "()Lcom/dxfeed/api/DXFeed;");
    jobject dxFeed = env->CallObjectMethod(dxEndpoint_, getFeedId);
    auto* pFeed = new DxFeed(env, dxFeed);
    env->DeleteLocalRef(dxFeed);
    return pFeed;
  }

  void DxEndpoint::close(JNIEnv* env) const {
    jmethodID closeMethodId = jni::safeGetMethodID(env, dxEndpointClass_, "close", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  void DxEndpoint::awaitNotConnected(JNIEnv* env) const {
    jmethodID closeMethodId = jni::safeGetMethodID(env, dxEndpointClass_, "awaitNotConnected", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  void DxEndpoint::addStateChangeListener(JNIEnv* env, dxfg_endpoint_state_change_listener_t* listener) {
    jclass pJclass = jni::safeFindClass(env, "Lcom/dxfeed/api/JniTest;");
    jmethodID addStateChangeListenerId = jni::safeGetStaticMethodID(env, pJclass, "addStateChangeEventListener",
                                         "(Lcom/dxfeed/api/DXEndpoint;J)V");
    env->CallStaticVoidMethod(pJclass, addStateChangeListenerId, dxEndpoint_, reinterpret_cast<jlong>(listener));
    env->DeleteLocalRef(pJclass);
  }

  void DxEndpoint::removeStateChangeListener(JNIEnv* env, dxfg_endpoint_state_change_listener_t* listener) {
    jclass pJclass = jni::safeFindClass(env, "Lcom/dxfeed/api/JniTest;");
    jmethodID removeStateChangeListenerId = jni::safeGetStaticMethodID(env, pJclass, "removeStateChangeEventListener",
                                                                    "(Lcom/dxfeed/api/DXEndpoint;J)V");
    env->CallStaticVoidMethod(pJclass, removeStateChangeListenerId, dxEndpoint_, reinterpret_cast<jlong>(listener));
    env->DeleteLocalRef(pJclass);
  }
}