// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_

#include <jni.h>
#include <cstdint>
#include "api/dxfg_endpoint.h"
#include "api/dxfg_events.h"
#include "DxSubscription.hpp"
#include "DxFeed.h"

namespace dxfeed {
  typedef void(* OnCloseHandler)(jobject clazz);

  struct DxEndpoint final {
    DxEndpoint(JNIEnv* env, dxfeed::OnCloseHandler onClose);
    ~DxEndpoint();

    DxEndpoint(const DxEndpoint& other) = delete;
    DxEndpoint(DxEndpoint&& other) = delete;
    DxEndpoint& operator=(const DxEndpoint& other) = delete;
    DxEndpoint& operator=(DxEndpoint&& other) = delete;

    int32_t connect(const char* address);
    DxFeed* getFeed();
  private:
    jobject createDxEndpointBuilder();
    jobject createDxEndpoint(jobject dxEndpointBuilder);

    jobject dxEndpoint_ = nullptr;
    JNIEnv* env_ = nullptr;
    const dxfeed::OnCloseHandler onClose_ = nullptr;
    jclass dxEndpointClass_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
