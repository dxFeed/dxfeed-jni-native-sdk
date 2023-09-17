// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_TRADEMAPPING_H
#define NATIVE_JNI_SDK_TRADEMAPPING_H

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct TradeMapping {
    static dxfg_trade_base_t* toTradeBase(ByteReader& reader);
    static void fromTradeBase(dxfg_trade_base_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_TRADEMAPPING_H
