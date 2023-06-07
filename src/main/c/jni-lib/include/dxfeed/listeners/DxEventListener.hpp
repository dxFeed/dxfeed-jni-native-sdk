// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_

#include <jni.h>

#include "api/dxfg_subscription.h"

namespace dxfeed {
  struct DxEventListener {
    constexpr static const char JAVA_CLASS_NAME[] = "com.dxfeed.api.JNIDXFeedEventListener";

    DxEventListener(JNIEnv* env, dxfg_feed_event_listener_function userFunc, void* userData);
    jobject getJavaHandle() const;
    void callUserFunc(graal_isolatethread_t* thread, dxfg_event_type_list* events);
    void removeFromJava();
    void closeFromNative();
  private:
    ~DxEventListener();

    jobject eventListener_ = nullptr;
    dxfg_feed_event_listener_function userFunc_ = nullptr;
    void* userData_ = nullptr;
    bool removedFromJava_ = false;
    bool deletedFromNative_ = false;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXEVENTLISTENER_H_
