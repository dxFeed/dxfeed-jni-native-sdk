// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_

#include <jni.h>

#include "api/dxfg_subscription.h"

namespace dxfeed {
  struct DxEventListener {
    static DxEventListener* create(JNIEnv* env, dxfg_feed_event_listener_function callback, void* userData);
  private:
    explicit DxEventListener(jlong listenerId);
    jobject dummy_ = nullptr;
  public:
    const jlong javaListenerId_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_
