// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_UNDERLYINGMAPPING_H
#define NATIVE_JNI_SDK_UNDERLYINGMAPPING_H

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct UnderlyingMapping {
    static dxfg_underlying_t* toUnderlying(ByteReader& reader);
    static void fromUnderlying(dxfg_underlying_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_UNDERLYINGMAPPING_H
