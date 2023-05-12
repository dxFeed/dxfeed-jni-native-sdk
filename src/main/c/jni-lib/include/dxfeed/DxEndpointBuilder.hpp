// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_

#include <jni.h>
#include <cstdint>

#include "api/dxfg_endpoint.h"
#include "api/dxfg_events.h"
#include "DxSubscription.hpp"
#include "DxFeed.hpp"
#include "DxEndpoint.hpp"

namespace dxfeed {
  struct DxEndpointBuilder final {
    explicit DxEndpointBuilder(JNIEnv* env);
    ~DxEndpointBuilder();

    DxEndpointBuilder(const DxEndpointBuilder& other) = delete;
    DxEndpointBuilder(DxEndpointBuilder&& other) = delete;
    DxEndpointBuilder& operator=(const DxEndpointBuilder& other) = delete;
    DxEndpointBuilder& operator=(DxEndpointBuilder&& other) = delete;

    DxEndpoint* build(JNIEnv* env);
    void withRole(JNIEnv* env, dxfg_endpoint_role_t role);
    void withName(JNIEnv* env, const char* name);
    void withProperty(JNIEnv* env, const char* key, const char* value);
    void withProperties(JNIEnv* env, const char* file_path);
    void supportsProperty(JNIEnv* env, const char* key);
  private:
    static jobject rebuild(JNIEnv* env, jobject oldBuilder, jobject newBuilder);

    jobject dxEndpointBuilder_ = nullptr;
    jclass dxEndpointBuilderClass_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_
