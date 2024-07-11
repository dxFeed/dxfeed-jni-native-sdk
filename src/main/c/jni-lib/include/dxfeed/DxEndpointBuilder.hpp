// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_

#include <jni.h>
#include <cstdint>

#include "api/dxfg_endpoint.h"

namespace dxfeed {
  struct DxEndpoint;

  struct DxEndpointBuilder final {
    constexpr static const char JAVA_CLASS_NAME[] = "com.dxfeed.api.impl.DXEndpointImpl$BuilderImpl";

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
    bool supportsProperty(JNIEnv* env, const char* key);
  private:
    jobject dxEndpointBuilder_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_
