// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NATIVE_JNI_SDK_NATIVEEVENTSLIST_H
#define NATIVE_JNI_SDK_NATIVEEVENTSLIST_H

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
    jobject fromNativeEventsList(dxfg_event_type_list* pList);
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
    jmethodID toList_;

    constexpr static const char NATIVE_EVENTS_LIST_JNI_CLASS_NAME[] = "com/dxfeed/api/NativeEventsList";
  };
}

#endif //NATIVE_JNI_SDK_NATIVEEVENTSLIST_H