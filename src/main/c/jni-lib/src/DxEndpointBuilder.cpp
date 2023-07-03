// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed {
  using namespace jni;
  
  DxEndpointBuilder::DxEndpointBuilder(JNIEnv* env) {
    jclass dxEndpointClass = safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint;");
    javaLogger->info("dxEndpointClass: %", dxEndpointClass);
    jmethodID newBuilderMethodId = safeGetStaticMethodID(env, dxEndpointClass, "newBuilder",
                                                              "()Lcom/dxfeed/api/DXEndpoint$Builder;");
    javaLogger->info("newBuilderMethodId: %", dxEndpointClass);
    jobject pDxEndpointBuilder = env->CallStaticObjectMethod(dxEndpointClass, newBuilderMethodId);
    dxEndpointBuilder_ = env->NewGlobalRef(pDxEndpointBuilder);
    env->DeleteLocalRef(pDxEndpointBuilder);
    javaLogger->info("dxEndpointBuilder_: %", dxEndpointBuilder_);
    dxEndpointBuilderClass_ = env->GetObjectClass(dxEndpointBuilder_);
    javaLogger->info("dxEndpointBuilderClass: %", dxEndpointBuilderClass_);
    env->DeleteLocalRef(dxEndpointClass);
  }

  DxEndpointBuilder::~DxEndpointBuilder() {
    internal::jniEnv->DeleteGlobalRef(dxEndpointBuilder_);
  }

  DxEndpoint* DxEndpointBuilder::build(JNIEnv* env) {
    jmethodID buildId = safeGetMethodID(env, dxEndpointBuilderClass_, "build", "()Lcom/dxfeed/api/DXEndpoint;");
    javaLogger->info("buildId: %", buildId);
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpointBuilder_, buildId);
    javaLogger->info("DxEndpoint OBJECT: %", pDxEndpoint);
    auto* pEndpoint = new DxEndpoint(env, pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return pEndpoint;
  }

  void DxEndpointBuilder::withRole(JNIEnv* env, dxfg_endpoint_role_t role) {
    jmethodID withRoleId = safeGetMethodID(env, dxEndpointBuilderClass_, "withRole",
                                             "(Lcom/dxfeed/api/DXEndpoint$Role;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    // todo: extract method for RoleId
    javaLogger->info("withRoleId: %", withRoleId);
    jclass jRoleClass = safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint$Role;");
    javaLogger->info("jRoleClass: %", jRoleClass);
    jmethodID valuesId = safeGetStaticMethodID(env, jRoleClass, "values", "()[Lcom/dxfeed/api/DXEndpoint$Role;");
    javaLogger->info("valuesId: %", valuesId);
    auto jRoleArray = r_cast<jobjectArray>(env->CallStaticObjectMethod(jRoleClass, valuesId));
    jobject jRole = env->GetObjectArrayElement(jRoleArray, role);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withRoleId, jRole);
    env->DeleteLocalRef(jRole);
    env->DeleteLocalRef(jRoleArray);
    env->DeleteLocalRef(jRoleClass);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
    env->DeleteLocalRef(newBuilder);
  }

  void DxEndpointBuilder::withName(JNIEnv* env, const char* name) {
    jmethodID withNameId = safeGetMethodID(env, dxEndpointBuilderClass_, "withName",
                                             "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    javaLogger->info("withNameId: %", withNameId);
    jstring jName = env->NewStringUTF(name);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withNameId, jName);
    env->DeleteLocalRef(jName);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
    env->DeleteLocalRef(newBuilder);
  }

  void DxEndpointBuilder::withProperty(JNIEnv* env, const char* key, const char* value) {
    jmethodID withPropertyId = safeGetMethodID(env, dxEndpointBuilderClass_, "withProperty",
                                                "(Ljava/lang/String;Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    javaLogger->info("withPropertyId: %", withPropertyId);
    jstring jKey = env->NewStringUTF(key);
    jstring jValue = env->NewStringUTF(value);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withPropertyId, jKey, jValue);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jValue);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
    env->DeleteLocalRef(newBuilder);
  }

  void DxEndpointBuilder::withProperties(JNIEnv* env, const char* filePath) {
    jclass jPropertiesClass = safeFindClass(env, "Ljava/util/Properties;");
    javaLogger->info("jPropertiesClass: %", jPropertiesClass);
    jmethodID loadId = safeGetMethodID(env, jPropertiesClass, "load", "(Ljava/io/InputStream;)V");
    javaLogger->info("loadId: %", loadId);
    env->DeleteLocalRef(jPropertiesClass);
    //java/util/Properties;
  }

  void DxEndpointBuilder::supportsProperty(JNIEnv* env, const char* key) {
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