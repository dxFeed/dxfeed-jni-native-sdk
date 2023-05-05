//
// Created by Andrey.Mikhalev on 5/3/23.
//

#include <iostream>
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"

namespace dxfeed {
  DxEndpointBuilder::DxEndpointBuilder(JNIEnv* env) :
    env_(env)
  {
    jclass dxEndpointClass = jni::safeFindClass(env_, "Lcom/dxfeed/api/DXEndpoint;");
    std::cout << "dxEndpointClass: " << dxEndpointClass << "\n";
    jmethodID newBuilderMethodId = jni::safeGetStaticMethodID(env_, dxEndpointClass, "newBuilder",
                                                              "()Lcom/dxfeed/api/DXEndpoint$Builder;");
    std::cout << "newBuilderMethodId: " << dxEndpointClass << "\n";
    dxEndpointBuilder_ = env_->NewGlobalRef(env_->CallStaticObjectMethod(dxEndpointClass, newBuilderMethodId));
    std::cout << "dxEndpointBuilder_: " << dxEndpointBuilder_ << "\n";
    dxEndpointBuilderClass_ = env->GetObjectClass(dxEndpointBuilder_);
    std::cout << "dxEndpointBuilderClass: " << dxEndpointBuilderClass_ << "\n";
    env_->DeleteLocalRef(dxEndpointClass);
  }

  DxEndpointBuilder::~DxEndpointBuilder() {
    env_->DeleteGlobalRef(dxEndpointBuilder_);
  }

  DxEndpoint* DxEndpointBuilder::build(JNIEnv* env) {
    jmethodID buildId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "build", "()Lcom/dxfeed/api/DXEndpoint;");
    std::cout << "buildId: " << buildId << "\n";
    jobject pJobject = env->CallObjectMethod(dxEndpointBuilder_, buildId);
    std::cout << "DxEndpoint OBJECT: " << pJobject << "\n";
    return new DxEndpoint(env, pJobject);
  }

  void DxEndpointBuilder::withRole(JNIEnv* env, dxfg_endpoint_role_t role) {
    jmethodID withRoleId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "withRole",
                                             "(Lcom/dxfeed/api/DXEndpoint$Role;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    std::cout << "withRoleId: " << withRoleId << "\n";
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withRoleId, role);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
  }

  void DxEndpointBuilder::withName(JNIEnv* env, const char* name) {
    jmethodID withNameId = jni::safeGetMethodID(env, dxEndpointBuilderClass_, "withName",
                                             "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint$Builder;");
    std::cout << "withNameId: " << withNameId << "\n";
    jstring jName = env->NewStringUTF(name);
    jobject newBuilder = env->CallObjectMethod(dxEndpointBuilder_, withNameId, jName);
    env->DeleteLocalRef(jName);
    dxEndpointBuilder_ = rebuild(env, dxEndpointBuilder_, newBuilder);
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
  }

  void DxEndpointBuilder::withProperties(JNIEnv* env, const char* file_path) {
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