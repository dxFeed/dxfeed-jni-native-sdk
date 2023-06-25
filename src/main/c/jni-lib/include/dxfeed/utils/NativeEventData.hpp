// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_NATIVEEVENTDATA_H
#define NATIVE_JNI_SDK_NATIVEEVENTDATA_H

#include <jni.h>
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventData {
    explicit NativeEventData(JNIEnv* env);
    ~NativeEventData();

    void toNativeEvent(jobject nativeEventData, dxfg_event_type_t** ppEventType);
  private:
    JNIEnv* env_;
    jclass dxNativeEventDataClass_;
    jfieldID byteData_;
    jfieldID doubleData_;
  };
}

#endif //NATIVE_JNI_SDK_NATIVEEVENTDATA_H