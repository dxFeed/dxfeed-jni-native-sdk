// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxPublisher.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
using namespace jni;

DxEndpoint::DxEndpoint(JNIEnv *env, jobject dxEndpoint) {
    dxEndpoint_ = env->NewGlobalRef(dxEndpoint);
}

DxEndpoint::~DxEndpoint() {
    internal::jniEnv->DeleteGlobalRef(dxEndpoint_);
}

jobject DxEndpoint::newBuilder(JNIEnv *env) {
    auto jDxEndpointClass = safeFindClass(env, DxEndpoint::DX_ENDPOINT_CLASS_NAME);
    const char *methodName = "newBuilder";
    const char *methodSignature = "()Lcom/dxfeed/api/DXEndpoint$Builder;";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointClass, methodName, methodSignature);
    auto builder = checkedCallStaticObjectMethod(env, jDxEndpointClass, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
    return builder;
}

DxEndpoint *DxEndpoint::getInstance(JNIEnv *env) {
    auto jDxEndpointClass = safeFindClass(env, DX_ENDPOINT_CLASS_NAME);
    const char *methodName = "getInstance";
    const char *methodSignature = "()Lcom/dxfeed/api/DXEndpoint;";
    auto getInstanceId = safeGetStaticMethodID(env, jDxEndpointClass, methodName, methodSignature);
    auto jDxEndpoint = checkedCallStaticObjectMethod(env, jDxEndpointClass, getInstanceId);
    DxEndpoint *result = nullptr;
    if (jDxEndpoint) {
        result = new DxEndpoint(env, jDxEndpoint);
        env->DeleteLocalRef(jDxEndpoint);
    }
    env->DeleteLocalRef(jDxEndpointClass);
    return result;
}

DxEndpoint *DxEndpoint::getInstance(JNIEnv *env, dxfg_endpoint_role_t dxfgEndpointRole) {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char *methodName = "getInstance";
    const char *methodSignature = "(I)Lcom/dxfeed/api/DXEndpoint;";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    auto jDxEndpoint = checkedCallStaticObjectMethod(env, jDxEndpointJniClass, methodId, dxfgEndpointRole);
    DxEndpoint *result = nullptr;
    if (jDxEndpoint) {
        result = new DxEndpoint(env, jDxEndpoint);
        env->DeleteLocalRef(jDxEndpoint);
    }
    env->DeleteLocalRef(jDxEndpointJniClass);
    return result;
}

DxFeed *DxEndpoint::getFeed(JNIEnv *env) {
    if (dxFeed_ == nullptr) {
        auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
        const char *methodName = "getFeed";
        const char *methodSignature = "()Lcom/dxfeed/api/DXFeed;";
        auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
        auto jDxFeed = checkedCallObjectMethod(env, dxEndpoint_, methodId);
        dxFeed_ = new DxFeed(env, jDxFeed);
        env->DeleteLocalRef(jDxFeed);
        env->DeleteLocalRef(jDxEndpointClass);
    }
    return dxFeed_;
}

DxPublisher *DxEndpoint::getPublisher(JNIEnv *env) const {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "getPublisher";
    const char *methodSignature = "()Lcom/dxfeed/api/DXPublisher;";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    auto jDxPublisher = checkedCallObjectMethod(env, dxEndpoint_, methodId);
    auto *pPublisher = new DxPublisher(env, jDxPublisher);
    env->DeleteLocalRef(jDxPublisher);
    env->DeleteLocalRef(jDxEndpointClass);
    return pPublisher;
}

dxfg_endpoint_role_t DxEndpoint::getRole(JNIEnv *env) const {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char *methodName = "getRole";
    const char *methodSignature = "(Lcom/dxfeed/api/DXEndpoint)I;";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    auto role = env->CallIntMethod(jDxEndpointJniClass, methodId, dxEndpoint_);
    env->DeleteLocalRef(jDxEndpointJniClass);
    return static_cast<dxfg_endpoint_role_t>(role);
}

dxfg_endpoint_state_t DxEndpoint::getState(JNIEnv *env) const {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char *methodName = "getState";
    const char *methodSignature = "(Lcom/dxfeed/api/DXEndpoint)I;";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    auto role = env->CallStaticIntMethod(jDxEndpointJniClass, methodId, dxEndpoint_);
    env->DeleteLocalRef(jDxEndpointJniClass);
    return static_cast<dxfg_endpoint_state_t>(role);
}

dxfg_event_clazz_list_t *DxEndpoint::getEventTypes(JNIEnv *env) const {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char *methodName = "getEventTypes";
    const char *methodSignature = "(Lcom/dxfeed/api/DXEndpoint;)[B";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);

    auto jByteArray =
        checkedCallStaticObjectMethodAndCastTo<jbyteArray>(env, jDxEndpointJniClass, methodId, dxEndpoint_);
    jint size = env->GetArrayLength(jByteArray);
    auto *pEventTypeData = r_cast<char *>(env->GetPrimitiveArrayCritical(jByteArray, 0));

    auto result = new dxfg_event_clazz_list_t();
    result->size = size;
    result->elements = new dxfg_event_clazz_t *[size];
    for (int i = 0; i < size; ++i) {
        auto *pClazz = new dxfg_event_clazz_t{static_cast<dxfg_event_clazz_t>(pEventTypeData[i])};
        result->elements[i] = pClazz;
    }

    env->ReleasePrimitiveArrayCritical(jByteArray, pEventTypeData, 0);
    env->DeleteLocalRef(jByteArray);
    env->DeleteLocalRef(jDxEndpointJniClass);

    return result;
}

int32_t DxEndpoint::user(JNIEnv *env, const char *userName) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "user";
    const char *methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    auto jUserName = env->NewStringUTF(userName);
    auto jDxEndpoint = checkedCallObjectMethod(env, dxEndpoint_, methodId, jUserName);
    env->DeleteLocalRef(jUserName);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointClass);
    return JNI_OK;
}

int32_t DxEndpoint::password(JNIEnv *env, const char *password) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "password";
    const char *methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    auto jPassword = env->NewStringUTF(password);
    auto jDxEndpoint = checkedCallObjectMethod(env, dxEndpoint_, methodId, jPassword);
    env->DeleteLocalRef(jPassword);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointClass);
    return JNI_OK;
}

int32_t DxEndpoint::connect(JNIEnv *env, const char *address) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "connect";
    const char *methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    auto jAddress = env->NewStringUTF(address);
    auto jDxEndpoint = checkedCallObjectMethod(env, dxEndpoint_, methodId, jAddress);
    env->DeleteLocalRef(jAddress);
    env->DeleteLocalRef(jDxEndpoint);
    env->DeleteLocalRef(jDxEndpointClass);
    return JNI_OK;
}

void DxEndpoint::reconnect(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "reconnect";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::disconnect(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "disconnect";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::disconnectAndClear(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "disconnectAndClear";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::close(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "close";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::closeAndAwaitTermination(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "closeAndAwaitTermination";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::awaitProcessed(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "awaitProcessed";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::awaitNotConnected(JNIEnv *env) {
    auto jDxEndpointClass = env->GetObjectClass(dxEndpoint_);
    const char *methodName = "awaitNotConnected";
    const char *methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxEndpointClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxEndpoint_, methodId);
    env->DeleteLocalRef(jDxEndpointClass);
}

void DxEndpoint::addStateChangeListener(JNIEnv *env, DxStateChangeListener *listener) {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char *methodName = "addStateChangeEventListener";
    const char *methodSignature = "(Lcom/dxfeed/api/DXEndpoint;J)V";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    checkedCallStaticVoidMethod(env, jDxEndpointJniClass, methodId, dxEndpoint_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxEndpointJniClass);
}

void DxEndpoint::removeStateChangeListener(JNIEnv *env, DxStateChangeListener *listener) {
    auto jDxEndpointJniClass = safeFindClass(env, DX_JNI_ENDPOINT_CLASS_NAME);
    const char *methodName = "removeStateChangeEventListener";
    const char *methodSignature = "(Lcom/dxfeed/api/DXEndpoint;J)V";
    auto methodId = safeGetStaticMethodID(env, jDxEndpointJniClass, methodName, methodSignature);
    checkedCallStaticVoidMethod(env, jDxEndpointJniClass, methodId, dxEndpoint_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxEndpointJniClass);
}
} // namespace dxfeed