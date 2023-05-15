// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_

#include <jni.h>
#include <cstdint>

#include "api/dxfg_endpoint.h"
#include "api/dxfg_events.h"
#include "DxSubscription.hpp"
#include "DxFeed.hpp"

namespace dxfeed {
  struct DxEndpoint final {
    explicit DxEndpoint(JNIEnv* env, jobject dxEndpoint);
    ~DxEndpoint();

    DxEndpoint(const DxEndpoint& other) = delete;
    DxEndpoint(DxEndpoint&& other) = delete;
    DxEndpoint& operator=(const DxEndpoint& other) = delete;
    DxEndpoint& operator=(DxEndpoint&& other) = delete;

    int32_t connect(JNIEnv* env, const char* address);
    DxFeed* getFeed(JNIEnv* env) const;
    void close(JNIEnv* env) const;
    void awaitNotConnected(JNIEnv* env) const;
    void addStateChangeListener(JNIEnv* env, dxfg_endpoint_state_change_listener_t* listener);
    void removeStateChangeListener(JNIEnv* env, dxfg_endpoint_state_change_listener_t* listener);
  private:
    jobject dxEndpoint_ = nullptr;
    jclass dxEndpointClass_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
