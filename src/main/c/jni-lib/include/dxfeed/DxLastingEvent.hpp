// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXLASTINGEVENT_H_
#define DXFEED_JNI_NATIVE_SDK_DXLASTINGEVENT_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {

  struct DxLastingEvent {
    DxLastingEvent(dxfg_event_type_t eventType, jlong nativeHandlerId);

    DxLastingEvent(const DxLastingEvent& other) = delete;
    DxLastingEvent(DxLastingEvent&& other) = delete;
    DxLastingEvent& operator=(const DxLastingEvent& other) = delete;
    DxLastingEvent& operator=(DxLastingEvent&& other) = delete;

    dxfg_event_type_t eventType;
    jlong nativeHandlerId;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXLASTINGEVENT_H_
