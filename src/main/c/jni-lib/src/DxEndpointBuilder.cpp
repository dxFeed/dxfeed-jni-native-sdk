// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed {
  using namespace jni;
  
  DxEndpointBuilder::DxEndpointBuilder(JNIEnv* env) {
    jclass jDxEndpointClass = safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint;");
//    javaLogger->info("jDxEndpointClass: %", jDxEndpointClass);
    const char* methodName = "newBuilder";
    const char* methodSignature = "()Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxEndpointClass, methodName, methodSignature);
//    javaLogger->info("methodId: %", jDxEndpointClass);
    jobject jDxEndpointBuilder = env->CallStaticObjectMethod(jDxEndpointClass, methodId);
    dxEndpointBuilder_ = env->NewGlobalRef(jDxEndpointBuilder);
    env->DeleteLocalRef(jDxEndpointBuilder);
    javaLogger->info("dxEndpointBuilder_: %", dxEndpointBuilder_);
    dxEndpointBuilderClass_ = env->GetObjectClass(dxEndpointBuilder_);
    javaLogger->info("dxEndpointBuilderClass: %", dxEndpointBuilderClass_);
    env->DeleteLocalRef(jDxEndpointClass);
  }

  DxEndpointBuilder::~DxEndpointBuilder() {
    internal::jniEnv->DeleteGlobalRef(dxEndpointBuilder_);
  }

  DxEndpoint* DxEndpointBuilder::build(JNIEnv* env) {
    const char* methodName = "build";
    const char* methodSignature = "()Lcom/dxfeed/api/DXEndpoint;";
    jmethodID methodId = safeGetMethodID(env, dxEndpointBuilderClass_, methodName, methodSignature);
    jobject jDxEndpoint = env->CallObjectMethod(dxEndpointBuilder_, methodId);
    javaLogger->info("DxEndpoint OBJECT: %", jDxEndpoint);
    auto* pEndpoint = new DxEndpoint(env, jDxEndpoint);
    env->DeleteLocalRef(jDxEndpoint);
    return pEndpoint;
  }

  void DxEndpointBuilder::withRole(JNIEnv* env, dxfg_endpoint_role_t role) {
    const char* methodName = "withRole";
    const char* methodSignature = "(Lcom/dxfeed/api/DXEndpoint$Role;)Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetMethodID(env, dxEndpointBuilderClass_, methodName, methodSignature);
    jobjectArray jRoleEnumValues = roleEnumValues(env);
    jobject jRole = env->GetObjectArrayElement(jRoleEnumValues, role);
    jobject jNewBuilder = env->CallObjectMethod(dxEndpointBuilder_, methodId, jRole);
    env->DeleteLocalRef(jRole);
    env->DeleteLocalRef(jRoleEnumValues);
    
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, jNewBuilder);
    env->DeleteLocalRef(jNewBuilder);
  }

  jobjectArray DxEndpointBuilder::roleEnumValues(JNIEnv* env) {
    jclass jRoleClass = safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint$Role;");
    const char* methodName = "values";
    const char* methodSignature = "()[Lcom/dxfeed/api/DXEndpoint$Role;";
    jmethodID methodId = safeGetStaticMethodID(env, jRoleClass, methodName, methodSignature);    
    env->DeleteLocalRef(jRoleClass);
    return r_cast<jobjectArray>(env->CallStaticObjectMethod(jRoleClass, methodId));
  }

  void DxEndpointBuilder::withName(JNIEnv* env, const char* name) {
    const char* methodName = "withName";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetMethodID(env, dxEndpointBuilderClass_, methodName, methodSignature);
    jstring jName = env->NewStringUTF(name);
    jobject jNewBuilder = env->CallObjectMethod(dxEndpointBuilder_, methodId, jName);
    env->DeleteLocalRef(jName);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, jNewBuilder);
    env->DeleteLocalRef(jNewBuilder);
  }

  void DxEndpointBuilder::withProperty(JNIEnv* env, const char* key, const char* value) {
    const char* methodName = "withProperty";
    const char* methodSignature = "(Ljava/lang/String;Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;";
    jmethodID methodId = safeGetMethodID(env, dxEndpointBuilderClass_, methodName, methodSignature);
    jstring jKey = env->NewStringUTF(key);
    jstring jValue = env->NewStringUTF(value);
    jobject jNewBuilder = env->CallObjectMethod(dxEndpointBuilder_, methodId, jKey, jValue);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jValue);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, jNewBuilder);
    env->DeleteLocalRef(jNewBuilder);
  }

  void DxEndpointBuilder::withProperties(JNIEnv* env, const char* filePath) {
    jclass jPropertiesClass = safeFindClass(env, "Ljava/util/Properties;");
    javaLogger->info("jPropertiesClass: %", jPropertiesClass);
    const char* methodName = "load";
    const char* methodSignature = "(Ljava/io/InputStream;)V";
    jmethodID loadId = safeGetMethodID(env, jPropertiesClass, methodName, methodSignature);    
    env->DeleteLocalRef(jPropertiesClass);
    //java/util/Properties;
  }

  bool DxEndpointBuilder::supportsProperty(JNIEnv* env, const char* key) {
    const char* methodName = "supportsProperty";
    const char* methodSignature = "(Ljava/lang/String;)Z";
    jmethodID methodId = safeGetMethodID(env, dxEndpointBuilderClass_, methodName, methodSignature);
    jstring jKey = env->NewStringUTF(key);
    jboolean result = env->CallBooleanMethod(dxEndpointBuilder_, methodId, jKey);
    env->DeleteLocalRef(jKey);
    return result;
  }

  jobject DxEndpointBuilder::rebuild(JNIEnv* env, jobject oldBuilder, jobject newBuilder) {
    if (newBuilder != nullptr) {
      javaLogger->info("newBuilder = %", newBuilder);
      env->DeleteGlobalRef(oldBuilder);
      return env->NewGlobalRef(newBuilder);
    } else {
      javaLogger->error("Can't build DxEndpointBuilder!");
      return oldBuilder;
    }
  }

}