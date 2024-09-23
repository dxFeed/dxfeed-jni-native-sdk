// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_SERIESMAPPING_HPP
#define NATIVE_JNI_SDK_SERIESMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct SeriesMapping {
    static dxfg_series_t *toSeries(ByteReader &reader);
    static void fromSeries(dxfg_series_t *eventType, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_SERIESMAPPING_HPP
