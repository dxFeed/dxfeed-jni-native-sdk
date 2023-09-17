// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_THEOPRICEMAPPING_H
#define NATIVE_JNI_SDK_THEOPRICEMAPPING_H

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteWriter;

  struct TheoPriceMapping {
    static dxfg_theo_price_t* toTheoPrice(const char** pByteData, const double** pDoubleData);
    static void fromTheoPrice(dxfg_theo_price_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_THEOPRICEMAPPING_H
