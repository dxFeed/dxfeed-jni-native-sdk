// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_TRADEMAPPING_H
#define NATIVE_JNI_SDK_TRADEMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct TradeMapping {
    static dxfg_trade_base_t* toTradeBase(const char** pByteData, const double** pDoubleData);
    static void fromTradeBase(dxfg_trade_base_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_TRADEMAPPING_H
