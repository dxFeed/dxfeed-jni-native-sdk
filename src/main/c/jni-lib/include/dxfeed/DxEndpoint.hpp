// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
#define DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_

#include <jni.h>
#include <cstdint>

#include "api/dxfg_endpoint.h"
#include "DxPublisher.hpp"

namespace dxfeed {
  struct DxFeed;
  struct DxStateChangeListener;

  struct DxEndpoint final {
    constexpr static const char DX_JNI_ENDPOINT_CLASS_NAME[] = "com/dxfeed/api/DxEndpointJni";
    constexpr static const char JAVA_CLASS_NAME[] = "com.dxfeed.api.impl.DXEndpointImpl";

    explicit DxEndpoint(JNIEnv* env, jobject dxEndpoint);
    ~DxEndpoint();

    DxEndpoint(const DxEndpoint& other) = delete;
    DxEndpoint(DxEndpoint&& other) = delete;
    DxEndpoint& operator=(const DxEndpoint& other) = delete;
    DxEndpoint& operator=(DxEndpoint&& other) = delete;

    static jobject newBuilder(JNIEnv* env);
    static DxEndpoint* getInstance(JNIEnv* env);
    static DxEndpoint* getInstance(JNIEnv* env, dxfg_endpoint_role_t dxfgEndpointRole);

    DxFeed* getFeed(JNIEnv* env) const;
    DxPublisher* getPublisher(JNIEnv* env) const;
    dxfg_endpoint_role_t getRole(JNIEnv* pEnv) const;
    dxfg_endpoint_state_t getState(JNIEnv* pEnv) const;
    dxfg_event_clazz_list_t* getEventTypes(JNIEnv* env) const;

    int32_t user(JNIEnv* env, const char* userName);
    int32_t password(JNIEnv* env, const char* password);
    int32_t connect(JNIEnv* env, const char* address);
    void reconnect(JNIEnv* env);
    void disconnect(JNIEnv* env);
    void disconnectAndClear(JNIEnv* env);
    void close(JNIEnv* env);
    void closeAndAwaitTermination(JNIEnv* pEnv);
    void awaitProcessed(JNIEnv* env);
    void awaitNotConnected(JNIEnv* env);
    void addStateChangeListener(JNIEnv* env, DxStateChangeListener* listener);
    void removeStateChangeListener(JNIEnv* env, DxStateChangeListener* listener);
  private:
    jobject dxEndpoint_ = nullptr;

    constexpr static const char DX_ENDPOINT_CLASS_NAME[] = "com/dxfeed/api/DXEndpoint";
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
