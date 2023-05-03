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
    env_->DeleteLocalRef(dxEndpointClass);
  }

  DxEndpointBuilder::~DxEndpointBuilder() {
    env_->DeleteGlobalRef(dxEndpointBuilder_);
  }

  DxEndpoint* DxEndpointBuilder::build(JNIEnv* env) {
    jclass dxEndpointBuilderClass = env->GetObjectClass(dxEndpointBuilder_);
    std::cout << "dxEndpointBuilderClass: " << dxEndpointBuilderClass << "\n";
    jmethodID buildId = jni::safeGetMethodID(env, dxEndpointBuilderClass, "build", "()Lcom/dxfeed/api/DXEndpoint;");
    std::cout << "buildId: " << buildId << "\n";
    env->DeleteLocalRef(dxEndpointBuilderClass);
    jobject pJobject = env->CallObjectMethod(dxEndpointBuilder_, buildId);
    std::cout << "DxEndpoint OBJECT: " << pJobject << "\n";
    return new DxEndpoint(env, pJobject);
  }
}