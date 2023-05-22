// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_

#include <jni.h>
#include "api/dxfg_subscription.h"

namespace dxfeed {
  struct DxEventListener {
    DxEventListener(JNIEnv* env, dxfg_feed_event_listener_function userFunc, void* userData);
    ~DxEventListener();
    jobject getJavaHandle() const;
    void callUserFunc(graal_isolatethread_t* thread, dxfg_event_type_list* events);
  private:
    jobject eventListener_ = nullptr;
    dxfg_feed_event_listener_function userFunc_ = nullptr;
    void* userData_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_
