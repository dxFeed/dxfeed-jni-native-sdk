// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_NATIVEEVENTSLIST_H
#define NATIVE_JNI_SDK_NATIVEEVENTSLIST_H

#include "api/dxfg_events.h"
#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include <jni.h>

namespace dxfeed::jni {
struct NativeEventsList {
    explicit NativeEventsList(JNIEnv *env);
    ~NativeEventsList();

    void toNativeEvent(jobject nativeEventsList, dxfg_event_type_t **ppEventType);
    void toNativeEventsList(jobject nativeEventsList, dxfg_event_type_list **ppEventTypeList);

    static void initToListMethod(JNIEnv *env);
    static jobject fromNativeEventsList(JNIEnv *env, dxfg_event_type_list *pList);

  private:
    JNIEnv *env_;
    jclass dxNativeEventsListClass_;
    jclass dxByteBuffer_;
    jclass dxDoubleBuffer_;
    jfieldID byteBuffer_;
    jmethodID toByteData_;
    jfieldID doubleBuffer_;
    jmethodID toDoubleData_;
    jfieldID pEventTypes_;
    inline static jmethodID toList_;

    constexpr static const char NATIVE_EVENTS_LIST_JNI_CLASS_NAME[] = "com/dxfeed/api/NativeEventsList";
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_NATIVEEVENTSLIST_H