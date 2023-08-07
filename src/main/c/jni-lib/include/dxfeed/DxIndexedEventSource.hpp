// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_
#define DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_

#include <jni.h>
#include <string>

#include "api/dxfg_events.h"

namespace dxfeed {

  struct DxIndexedEventSource final {
    DxIndexedEventSource(JNIEnv* env, const char* name);

    DxIndexedEventSource(const DxIndexedEventSource& other) = delete;
    DxIndexedEventSource(DxIndexedEventSource&& other) = delete;
    DxIndexedEventSource& operator=(const DxIndexedEventSource& other) = delete;
    DxIndexedEventSource& operator=(DxIndexedEventSource&& other) = delete;

    jobject javaObject() const;
    void release(JNIEnv* env);

    static bool isSpecialSourceId(JNIEnv* env, int32_t index);
    static dxfg_indexed_event_source_t* createByEventType(JNIEnv* env, dxfg_event_type_t* pType);
  private:
    const char* getName(JNIEnv* env);
    DxIndexedEventSource(JNIEnv* env, int32_t sourceId);
    ~DxIndexedEventSource();

    dxfg_indexed_event_source_type_t type_;
    int32_t id_;
    const char* name_;
    jobject indexedEventSource_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_