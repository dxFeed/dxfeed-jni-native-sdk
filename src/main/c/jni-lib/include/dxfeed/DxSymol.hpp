// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_
#define DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {

  struct DxSymbol {
    DxSymbol(dxfg_symbol_type_t eventType, jlong nativeHandlerId);

    DxSymbol(const DxSymbol& other) = delete;
    DxSymbol(DxSymbol&& other) = delete;
    DxSymbol& operator=(const DxSymbol& other) = delete;
    DxSymbol& operator=(DxSymbol&& other) = delete;

    dxfg_symbol_type_t eventType;
    jlong nativeHandlerId;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_
