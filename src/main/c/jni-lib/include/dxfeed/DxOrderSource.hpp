// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_
#define DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_

#include <jni.h>
#include <string>

#include "api/dxfg_events.h"

namespace dxfeed {

  struct DxOrderSource final {
    DxOrderSource(JNIEnv* env, const char* name);

    DxOrderSource(const DxOrderSource& other) = delete;
    DxOrderSource(DxOrderSource&& other) = delete;
    DxOrderSource& operator=(const DxOrderSource& other) = delete;
    DxOrderSource& operator=(DxOrderSource&& other) = delete;

    void releaseJavaObject(JNIEnv* env);

    static bool isSpecialSourceId(JNIEnv* env, int32_t index);
    static dxfg_indexed_event_source_t* createOrderSourceById(JNIEnv* env, const int32_t sourceId);
  private:
    DxOrderSource(dxfg_indexed_event_source_type_t type, int32_t id, const char* name, jlong nativeHandleId);

    dxfg_indexed_event_source_type_t type_;
    int32_t id_;
    const char* name_;
    jlong nativeHandleId_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_ORDERSOURCE_H_