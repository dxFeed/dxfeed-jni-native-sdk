// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_

#include <jni.h>
#include <cstdint>

#include "api/dxfg_endpoint.h"
#include "api/dxfg_events.h"
#include "DxSubscription.hpp"
#include "DxFeed.h"
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
  private:

    jobject dxEndpointBuilder_ = nullptr;
    JNIEnv* env_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINTBUILDER_H_
