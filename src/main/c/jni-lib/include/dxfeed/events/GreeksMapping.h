// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_GREEKSMAPPING_H
#define NATIVE_JNI_SDK_GREEKSMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct GreeksMapping {
    static dxfg_greeks_t* toGreeks(const char** pByteData, const double** pDoubleData);
    static void fromGreeks(dxfg_greeks_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_GREEKSMAPPING_H
