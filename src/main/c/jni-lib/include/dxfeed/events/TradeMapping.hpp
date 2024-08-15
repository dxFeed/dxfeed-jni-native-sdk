// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_TRADEMAPPING_HPP
#define NATIVE_JNI_SDK_TRADEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct TradeMapping {
    static dxfg_trade_base_t *toTradeBase(ByteReader &reader, dxfg_event_clazz_t dxfgEventClazz);
    static void fromTradeBase(dxfg_trade_base_t *eventType, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_TRADEMAPPING_HPP
