// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_

#include <jni.h>

#include "api/dxfg_endpoint.h"

namespace dxfeed {
  struct DxStateChangeListener {
    constexpr static const char JAVA_CLASS_NAME[] = "com.dxfeed.api.JniTest$JNIPropertyChangeListener";

    DxStateChangeListener(JNIEnv* env, dxfg_endpoint_state_change_listener_func userFunc, void* userData);
    ~DxStateChangeListener();
    jobject getJavaHandle() const;
    void callUserFunc(graal_isolatethread_t* thread, int32_t oldState, int32_t newState);
  private:
    jobject stateChangeListener_ = nullptr;
    dxfg_endpoint_state_change_listener_func userFunc_ = nullptr;
    void* userData_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
