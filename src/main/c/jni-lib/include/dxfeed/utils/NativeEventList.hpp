// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_NATIVEEVENTLIST_H
#define NATIVE_JNI_SDK_NATIVEEVENTLIST_H

#include <jni.h>
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventList {
    explicit NativeEventList(JNIEnv* env);
    ~NativeEventList();

    void toNativeEvent(jobject nativeEventList, dxfg_event_type_t** ppEventType);
    void toNativeEventList(jobject nativeEventList, dxfg_event_type_list** ppEventTypeList);
  private:
    JNIEnv* env_;
    jclass dxNativeEventListClass_;
    jclass dxByteBuffer_;
    jclass dxDoubleBuffer_;
    jfieldID byteBuffer_;
    jmethodID toByteData_;
    jfieldID doubleBuffer_;
    jmethodID toDoubleData_;
    jfieldID pEventTypes_;
  };
}

#endif //NATIVE_JNI_SDK_NATIVEEVENTLIST_H