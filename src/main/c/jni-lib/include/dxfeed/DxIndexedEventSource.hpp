// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_
#define DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_

#include <jni.h>
#include <string>

#include "api/dxfg_events.h"

namespace dxfeed {

  struct DxIndexedEventSource final {
    DxIndexedEventSource(JNIEnv* env, dxfg_indexed_event_source_t* eventSource);
    DxIndexedEventSource(JNIEnv* env, const char* name);
    ~DxIndexedEventSource();

    DxIndexedEventSource(const DxIndexedEventSource& other) = delete;
    DxIndexedEventSource(DxIndexedEventSource&& other) = delete;
    DxIndexedEventSource& operator=(const DxIndexedEventSource& other) = delete;
    DxIndexedEventSource& operator=(DxIndexedEventSource&& other) = delete;

    jobject javaObject() const;

    static bool isSpecialSourceId(JNIEnv* env, int32_t index);
    static dxfg_indexed_event_source_t* createByEventType(JNIEnv* env, dxfg_event_type_t* pType);
  private:
    const char* getName(JNIEnv* env);
    DxIndexedEventSource(JNIEnv* env, int32_t sourceId);

    dxfg_indexed_event_source_type_t type_;
    int32_t id_;
    const char* name_;
    jobject indexedEventSource_;
    JNIEnv* env_;

    constexpr static const char DX_INDEXED_EVENT_SOURCE_CLASS_NAME[] = "com/dxfeed/event/IndexedEventSource";
    constexpr static const char DX_INDEXED_EVENT_SOURCE_JNI_CLASS_NAME[] = "com/dxfeed/api/DxIndexedEventSource";
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_