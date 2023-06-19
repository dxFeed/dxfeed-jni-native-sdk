// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

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

    void releaseJavaObject(graal_isolatethread_t* pEnv);
  private:
    dxfg_indexed_event_source_type_t type_;
    int32_t id_;
    const char* name_;
    jlong nativeHandleId_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_