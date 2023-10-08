// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_THEOPRICEMAPPING_HPP
#define NATIVE_JNI_SDK_THEOPRICEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct TheoPriceMapping {
    static dxfg_theo_price_t* toTheoPrice(ByteReader& reader);
    static void fromTheoPrice(dxfg_theo_price_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_THEOPRICEMAPPING_HPP
