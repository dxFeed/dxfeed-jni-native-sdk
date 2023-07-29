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

  dxfg_endpoint_role_t DxEndpoint::getRole(JNIEnv* env) const {
    auto dxEndpointClass = internal::dxJni->dxEndpointJniClass_;
    jmethodID getInstanceId = safeGetStaticMethodID(env, dxEndpointClass, "getRole", "(Lcom/dxfeed/api/DXEndpoint)I;");
    auto role = env->CallIntMethod(dxEndpointClass, getInstanceId, dxEndpoint_);
    return static_cast<dxfg_endpoint_role_t>(role);
  }

  int32_t DxEndpoint::user(JNIEnv* env, const char* userName) {
    jmethodID connectMethodId = safeGetMethodID(env, dxEndpointClass_, "user", "(Ljava/lang/String;)"
                                                                            "Lcom/dxfeed/api/DXEndpoint;");
    jstring jUserName = env->NewStringUTF(userName);
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpoint_, connectMethodId, jUserName);
    env->DeleteLocalRef(jUserName);
    // todo: CallObjectMethod returns this, so do we have to replace jobject?
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return JNI_OK;
  }

  int32_t DxEndpoint::password(JNIEnv* env, const char* password) {
    jmethodID connectMethodId = safeGetMethodID(env, dxEndpointClass_, "password", "(Ljava/lang/String;)"
                                                                                   "Lcom/dxfeed/api/DXEndpoint;");
    jstring jPassword = env->NewStringUTF(password);
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpoint_, connectMethodId, jPassword);
    env->DeleteLocalRef(jPassword);
    // todo: CallObjectMethod returns this, so do we have to replace jobject?
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return JNI_OK;
  }

  int32_t DxEndpoint::connect(JNIEnv* env, const char* address) {
    jmethodID connectMethodId = safeGetMethodID(env, dxEndpointClass_, "connect", "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;");
    jstring addr = env->NewStringUTF(address);
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpoint_, connectMethodId, addr);
    env->DeleteLocalRef(addr);
    // todo: CallObjectMethod returns this, so do we have to replace jobject?
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return JNI_OK;
  }

  void DxEndpoint::reconnect(JNIEnv* env) const {
    jmethodID reconnectMethodId = safeGetMethodID(env, dxEndpointClass_, "reconnect", "()V");
    env->CallVoidMethod(dxEndpoint_, reconnectMethodId);
  }

  void DxEndpoint::disconnect(JNIEnv* env) const {
    jmethodID reconnectMethodId = safeGetMethodID(env, dxEndpointClass_, "disconnect", "()V");
    env->CallVoidMethod(dxEndpoint_, reconnectMethodId);
  }

  void DxEndpoint::disconnectAndClear(JNIEnv* env) const {
    jmethodID reconnectMethodId = safeGetMethodID(env, dxEndpointClass_, "disconnectAndClear", "()V");
    env->CallVoidMethod(dxEndpoint_, reconnectMethodId);
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

  void DxEndpoint::closeAndAwaitTermination(JNIEnv* env) const {
    jmethodID closeMethodId = safeGetMethodID(env, dxEndpointClass_, "closeAndAwaitTermination", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  void DxEndpoint::awaitProcessed(JNIEnv* env) const {
    jmethodID closeMethodId = safeGetMethodID(env, dxEndpointClass_, "awaitProcessed", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  void DxEndpoint::awaitNotConnected(JNIEnv* env) const {
    jmethodID closeMethodId = safeGetMethodID(env, dxEndpointClass_, "awaitNotConnected", "()V");
    env->CallVoidMethod(dxEndpoint_, closeMethodId);
  }

  dxfg_endpoint_state_t DxEndpoint::getState(JNIEnv* env) const {
    auto dxEndpointClass = internal::dxJni->dxEndpointJniClass_;
    jmethodID getInstanceId = safeGetStaticMethodID(env, dxEndpointClass, "getState", "(Lcom/dxfeed/api/DXEndpoint)I;");
    auto role = env->CallIntMethod(dxEndpointClass, getInstanceId, dxEndpoint_);
    return static_cast<dxfg_endpoint_state_t>(role);
  }

  void DxEndpoint::addStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) const {
    auto dxEndpointClass = internal::dxJni->dxEndpointJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxEndpointClass, "addStateChangeEventListener",
                                               "(Lcom/dxfeed/api/DXEndpoint;J)V");
    env->CallStaticVoidMethod(dxEndpointClass, methodId, dxEndpoint_, listener->javaListenerId_);
  }

  void DxEndpoint::removeStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) const {
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

  DxEndpoint* DxEndpoint::getInstance(JNIEnv* env, dxfg_endpoint_role_t dxfgEndpointRole) {
    auto dxEndpointClass = internal::dxJni->dxEndpointJniClass_;
    jmethodID getInstanceId = safeGetStaticMethodID(env, dxEndpointClass, "getInstance",
                                                    "(I)Lcom/dxfeed/api/DXEndpoint;");
    jobject dxEndpoint = env->CallStaticObjectMethod(dxEndpointClass, getInstanceId, dxfgEndpointRole);
    DxEndpoint* result = nullptr;
    if (dxEndpoint) {
      result = new DxEndpoint(env, dxEndpoint);
      env->DeleteLocalRef(dxEndpoint);
    }
    env->DeleteLocalRef(dxEndpointClass);
    return result;
  }
}