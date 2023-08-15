// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"

namespace dxfeed {
  using namespace jni;

  DxEndpoint::DxEndpoint(JNIEnv* env, jobject dxEndpoint) {
    dxEndpoint_ = env->NewGlobalRef(dxEndpoint);
  }

  DxEndpoint::~DxEndpoint() {
    internal::jniEnv->DeleteGlobalRef(dxEndpoint_);
  }

  jobject DxEndpoint::newBuilder(JNIEnv* env) {
    auto jDxEndpointClass = safeFindClass(env, DxEndpoint::DX_ENDPOINT_CLASS_NAME);
    const char* methodName = "newBuilder";
    const char* methodSignature = "()Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointClass, methodName, methodSignature);
    jobject builder = env->CallStaticObjectMethod(jDxEndpointClass, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
    return builder;
  }

  DxEndpoint* DxEndpoint::getInstance(JNIEnv* env) {
    auto jDxEndpointClass = safeFindClass(env, DX_ENDPOINT_CLASS_NAME);
    const char* methodName = "getInstance";
    const char* methodSignature = "()Lcom/dxfeed/api/DXEndpoint;";
    jmethodID getInstanceId = safeGetStaticMethodID(env, jDxEndpointClass, methodName, methodSignature);
    jobject jDxEndpoint = env->CallStaticObjectMethod(jDxEndpointClass, getInstanceId);
    DxEndpoint* result = nullptr;
    if (jDxEndpoint) {
      result = new DxEndpoint(env, jDxEndpoint);
      env->DeleteLocalRef(jDxEndpoint);
    }
    env->DeleteLocalRef(jDxEndpointClass);
    return result;
  }

  DxEndpoint* DxEndpoint::getInstance(JNIEnv* env, dxfg_endpoint_role_t dxfgEndpointRole) {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "getInstance";
    const char* methodSignature = "(I)Lcom/dxfeed/api/DXEndpoint;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    jobject jDxEndpoint = env->CallStaticObjectMethod(jDxEndpointJniClass, methodId, dxfgEndpointRole);
    DxEndpoint* result = nullptr;
    if (jDxEndpoint) {
      result = new DxEndpoint(env, jDxEndpoint);
      env->DeleteLocalRef(jDxEndpoint);
    }
    env->DeleteLocalRef(jDxEndpointJniClass);
    return result;
  }

  DxFeed* DxEndpoint::getFeed(JNIEnv* env) const {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "getFeed";
    const char* methodSignature = "()Lcom/dxfeed/api/DXFeed;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    jobject jDxFeed = env->CallObjectMethod(dxEndpoint_, methodId);
    auto* pFeed = new DxFeed(env, jDxFeed);
    env->DeleteLocalRef(jDxFeed);
    env->DeleteLocalRef(jDxEndpointClass);
    return pFeed;
  }

  dxfg_endpoint_role_t DxEndpoint::getRole(JNIEnv* env) const {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "getRole";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint)I;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    auto role = env->CallIntMethod(jDxEndpointJniClass, methodId, dxEndpoint_);
    env->DeleteLocalRef(jDxEndpointJniClass);
    return static_cast<dxfg_endpoint_role_t>(role);
  }

  dxfg_endpoint_state_t DxEndpoint::getState(JNIEnv* env) const {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "getState";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint)I;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    auto role = env->CallStaticIntMethod(jDxEndpointJniClass, methodId, dxEndpoint_);
    env->DeleteLocalRef(jDxEndpointJniClass);
    return static_cast<dxfg_endpoint_state_t>(role);
  }

  // todo: check
  dxfg_event_clazz_list_t* DxEndpoint::getEventTypes(JNIEnv* env) const {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "getEventTypes";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint;)[Z";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);

    auto jByteArray = r_cast <jbyteArray>(env->CallStaticObjectMethod(jDxEndpointJniClass, methodId, dxEndpoint_));
    jint size = env->GetArrayLength(jByteArray);
    auto* pEventTypeData = r_cast<char*>(env->GetPrimitiveArrayCritical(jByteArray, 0));

    auto result = new dxfg_event_clazz_list_t();
    result->size = size;
    result->elements = new dxfg_event_clazz_t*[size];
    for (int i = 0; i < size; ++i) {
      auto* pClazz = new dxfg_event_clazz_t { static_cast<dxfg_event_clazz_t>(pEventTypeData[i]) };
      result->elements[i] = pClazz;
    }

    env->ReleasePrimitiveArrayCritical(jByteArray, pEventTypeData, 0);
    env->DeleteLocalRef(jByteArray);
    env->DeleteLocalRef(jDxEndpointJniClass);

    return result;
  }

  int32_t DxEndpoint::user(JNIEnv* env, const char* userName) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "user";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    jstring jUserName = env->NewStringUTF(userName);
    jobject jDxEndpoint = env->CallObjectMethod(dxEndpoint_, methodId, jUserName);
    env->DeleteLocalRef(jUserName);
    // todo: CallObjectMethod returns this, so do we have to replace jobject?
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointClass);
    return JNI_OK;
  }

  int32_t DxEndpoint::password(JNIEnv* env, const char* password) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "password";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    jstring jPassword = env->NewStringUTF(password);
    jobject jDxEndpoint = env->CallObjectMethod(dxEndpoint_, methodId, jPassword);
    env->DeleteLocalRef(jPassword);
    // todo: CallObjectMethod returns this, so do we have to replace jobject?
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointClass);
    return JNI_OK;
  }

  int32_t DxEndpoint::connect(JNIEnv* env, const char* address) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "connect";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    jstring jAddress = env->NewStringUTF(address);
    jobject jDxEndpoint = env->CallObjectMethod(dxEndpoint_, methodId, jAddress);
    env->DeleteLocalRef(jAddress);
    // todo: CallObjectMethod returns this, so do we have to replace jobject?
    env->DeleteGlobalRef(dxEndpoint_);
    dxEndpoint_ = env->NewGlobalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointClass);
    return JNI_OK;
  }

  void DxEndpoint::reconnect(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "reconnect";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::disconnect(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "disconnect";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::disconnectAndClear(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "disconnectAndClear";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::close(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "close";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::closeAndAwaitTermination(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "closeAndAwaitTermination";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::awaitProcessed(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "awaitProcessed";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::awaitNotConnected(JNIEnv* env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char* methodName = "awaitNotConnected";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    env->CallVoidMethod(dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  void DxEndpoint::addStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "addStateChangeEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    env->CallStaticVoidMethod(jDxEndpointJniClass, methodId, dxEndpoint_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxEndpointJniClass);
  }

  void DxEndpoint::removeStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char* methodName = "removeStateChangeEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    env->CallStaticVoidMethod(jDxEndpointJniClass, methodId, dxEndpoint_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxEndpointJniClass);
  }
}