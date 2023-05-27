// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_

#include <jni.h>
#include <cstdint>

#include "api/dxfg_endpoint.h"

namespace dxfeed {
  struct DxFeed;
  struct DxStateChangeListener;

  struct DxEndpoint final {
    constexpr static const char JAVA_CLASS_NAME[] = "com.dxfeed.api.impl.DXEndpointImpl";
    constexpr static const char JNI_CLASS_NAME[] = "Lcom/dxfeed/api/DXEndpoint;";

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
    void addStateChangeListener(JNIEnv* env, DxStateChangeListener* listener);
    void removeStateChangeListener(JNIEnv* env, DxStateChangeListener* listener);

    static DxEndpoint* getInstance(JNIEnv* env);

  private:
    jobject dxEndpoint_ = nullptr;
    jclass dxEndpointClass_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
