// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_

#include <jni.h>

#include "api/dxfg_endpoint.h"

namespace dxfeed {
  struct DxStateChangeListener {
    static DxStateChangeListener* create(JNIEnv* env, dxfg_endpoint_state_change_listener_func callback, void*
    userData);
  private:
    explicit DxStateChangeListener(jlong listenerId);
    jobject dummy_ = nullptr;
  public:
    const jlong javaListenerId_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
