// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_UNDERLYINGMAPPING_H
#define NATIVE_JNI_SDK_UNDERLYINGMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  struct ByteWriter;

  struct UnderlyingMapping {
    static dxfg_underlying_t* toUnderlying(const char** pByteData, const double** pDoubleData);
    static void fromUnderlying(dxfg_underlying_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_UNDERLYINGMAPPING_H
