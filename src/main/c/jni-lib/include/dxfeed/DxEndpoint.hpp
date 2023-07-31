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

    explicit DxEndpoint(JNIEnv* env, jobject dxEndpoint);
    ~DxEndpoint();

    DxEndpoint(const DxEndpoint& other) = delete;
    DxEndpoint(DxEndpoint&& other) = delete;
    DxEndpoint& operator=(const DxEndpoint& other) = delete;
    DxEndpoint& operator=(DxEndpoint&& other) = delete;

    dxfg_endpoint_role_t getRole(JNIEnv* pEnv) const;
    int32_t user(JNIEnv* env, const char* userName);
    int32_t password(JNIEnv* env, const char* password);
    int32_t connect(JNIEnv* env, const char* address);
    void reconnect(JNIEnv* env) const;
    void disconnect(JNIEnv* env) const;
    void disconnectAndClear(JNIEnv* env) const;
    DxFeed* getFeed(JNIEnv* env) const;
    void close(JNIEnv* env) const;
    void closeAndAwaitTermination(JNIEnv* pEnv) const;
    void awaitProcessed(JNIEnv* env) const;
    void awaitNotConnected(JNIEnv* env) const;
    dxfg_endpoint_state_t getState(JNIEnv* pEnv) const;
    void addStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) const;
    void removeStateChangeListener(JNIEnv* env, DxStateChangeListener* listener) const;
    dxfg_event_clazz_list_t* getEventTypes(JNIEnv* env);

    static DxEndpoint* getInstance(JNIEnv* env);
    static DxEndpoint* getInstance(JNIEnv* env, dxfg_endpoint_role_t dxfgEndpointRole);

  private:
    jobject dxEndpoint_ = nullptr;
    jclass dxEndpointClass_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXENDPOINT_H_
