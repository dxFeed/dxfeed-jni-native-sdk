// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_QUOTEMAPPING_HPP
#define NATIVE_JNI_SDK_QUOTEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct QuoteMapping {
    static dxfg_quote_t *toQuote(ByteReader &reader);
    static void fromQuote(dxfg_quote_t *eventType, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_QUOTEMAPPING_HPP
