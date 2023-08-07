// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_NATIVEEVENTLIST_H
#define NATIVE_JNI_SDK_NATIVEEVENTLIST_H

#include <jni.h>
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventsList {
    explicit NativeEventsList(JNIEnv* env);
    ~NativeEventsList();

    void toNativeEvent(jobject nativeEventsList, dxfg_event_type_t** ppEventType);
    void toNativeEventsList(jobject nativeEventsList, dxfg_event_type_list** ppEventTypeList);
  private:
    JNIEnv* env_;
    jclass dxNativeEventsListClass_;
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