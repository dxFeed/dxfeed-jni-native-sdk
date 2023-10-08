// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_GREEKSMAPPING_HPP
#define NATIVE_JNI_SDK_GREEKSMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct GreeksMapping {
    static dxfg_greeks_t* toGreeks(ByteReader& reader);
    static void fromGreeks(dxfg_greeks_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_GREEKSMAPPING_HPP
