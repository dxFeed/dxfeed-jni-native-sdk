// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEndpointBuilder::DxEndpointBuilder(JNIEnv* env) {
    jclass dxEndpointClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint;");
    std::cout << "dxEndpointClass: " << dxEndpointClass << "\n";
    jmethodID newBuilderMethodId = jni::safeGetStaticMethodID(env, dxEndpointClass, "newBuilder",
                                                              "()Lcom/dxfeed/api/DXEndpoint$Builder;");
    std::cout << "newBuilderMethodId: " << dxEndpointClass << "\n";
    jobject pDxEndpointBuilder = env->CallStaticObjectMethod(dxEndpointClass, newBuilderMethodId);
    dxEndpointBuilder_ = env->NewGlobalRef(pDxEndpointBuilder);
    env->DeleteLocalRef(pDxEndpointBuilder);
    std::cout << "dxEndpointBuilder_: " << dxEndpointBuilder_ << "\n";
    dxEndpointBuilderClass_ = env->GetObjectClass(dxEndpointBuilder_);
    std::cout << "dxEndpointBuilderClass: " << dxEndpointBuilderClass_ << "\n";
    env->DeleteLocalRef(dxEndpointClass);
  }

  DxEndpointBuilder::~DxEndpointBuilder() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(dxEndpointBuilder_);
  }

  DxEndpoint* DxEndpointBuilder::build(JNIEnv* env) {
    jmethodID buildId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "build", "()Lcom/dxfeed/api/DXEndpoint;");
    std::cout << "buildId: " << buildId << "\n";
    jobject pDxEndpoint = env->CallObjectMethod(dxEndpointBuilder_, buildId);
    std::cout << "DxEndpoint OBJECT: " << pDxEndpoint << "\n";
    auto* pEndpoint = new DxEndpoint(env, pDxEndpoint);
    env->DeleteLocalRef(pDxEndpoint);
    return pEndpoint;
  }

  void DxEndpointBuilder::withRole(JNIEnv* env, dxfg_endpoint_role_t role) {
    jmethodID withRoleId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "withRole",
                                             "(Lcom/dxfeed/api/DXEndpoint$Role;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    // todo: extract method for RoleId
    std::cout << "withRoleId: " << withRoleId << "\n";
    jclass jRoleClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint$Role;");
    std::cout << "jRoleClass: " << jRoleClass << "\n";
    jmethodID valuesId = jni::safeGetStaticMethodID(env, jRoleClass, "values", "()[Lcom/dxfeed/api/DXEndpoint$Role;");
    std::cout << "valuesId: " << valuesId << "\n";
    auto jRoleArray = reinterpret_cast<jobjectArray>(env->CallStaticObjectMethod(jRoleClass, valuesId));
    jobject jRole = env->GetObjectArrayElement(jRoleArray, role);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withRoleId, jRole);
    env->DeleteLocalRef(jRole);
    env->DeleteLocalRef(jRoleArray);
    env->DeleteLocalRef(jRoleClass);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
    env->DeleteLocalRef(newBuilder);
  }

  void DxEndpointBuilder::withName(JNIEnv* env, const char* name) {
    jmethodID withNameId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "withName",
                                             "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    std::cout << "withNameId: " << withNameId << "\n";
    jstring jName = env->NewStringUTF(name);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withNameId, jName);
    env->DeleteLocalRef(jName);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
    env->DeleteLocalRef(newBuilder);
  }

  void DxEndpointBuilder::withProperty(JNIEnv* env, const char* key, const char* value) {
    jmethodID withPropertyId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "withProperty",
                                                "(Ljava/lang/String;Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    std::cout << "withPropertyId: " << withPropertyId << "\n";
    jstring jKey = env->NewStringUTF(key);
    jstring jValue = env->NewStringUTF(value);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withPropertyId, jKey, jValue);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(jValue);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
    env->DeleteLocalRef(newBuilder);
  }

  void DxEndpointBuilder::withProperties(JNIEnv* env, const char* filePath) {
    jclass jPropertiesClass = jni::safeFindClass(env, "Ljava/util/Properties;");
    std::cout << "jPropertiesClass: " << jPropertiesClass << "\n";
    jmethodID loadId = jni::safeGetMethodID(env, jPropertiesClass, "load", "(Ljava/io/InputStream;)V");
    std::cout << "loadId: " << loadId << "\n";
    env->DeleteLocalRef(jPropertiesClass);
    //java/util/Properties;
  }

  void DxEndpointBuilder::supportsProperty(JNIEnv* env, const char* key) {
  }

  jobject DxEndpointBuilder::rebuild(JNIEnv* env, jobject oldBuilder, jobject newBuilder) {
    if (newBuilder != nullptr) {
      std::cout << "newBuilder = " << newBuilder << "\n";
      env->DeleteGlobalRef(oldBuilder);
      return env->NewGlobalRef(newBuilder);
    } else {
      std::cerr << "Can't build DxEndpointBuilder!\n";
      return oldBuilder;
    }
  }
  
}