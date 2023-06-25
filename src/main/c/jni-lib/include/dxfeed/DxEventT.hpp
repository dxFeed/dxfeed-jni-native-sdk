// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXLASTINGEVENT_H_
#define DXFEED_JNI_NATIVE_SDK_DXLASTINGEVENT_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {

  struct DxEventT {
    DxEventT(dxfg_event_type_t eventType, const char* nativeHandlerId);

    DxEventT(const DxEventT& other) = delete;
    DxEventT(DxEventT&& other) = delete;
    DxEventT& operator=(const DxEventT& other) = delete;
    DxEventT& operator=(DxEventT&& other) = delete;

    dxfg_event_type_t eventType_;
    const char* symbol_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXLASTINGEVENT_H_
