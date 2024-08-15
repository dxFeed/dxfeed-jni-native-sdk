// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_CANDLEMAPPING_HPP
#define NATIVE_JNI_SDK_CANDLEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct CandleMapping {
    static dxfg_candle_t *toCandle(ByteReader &reader);
    static void fromCandle(dxfg_candle_t *eventType, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_CANDLEMAPPING_HPP
