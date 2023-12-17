// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
