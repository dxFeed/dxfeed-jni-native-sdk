// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_MESSAGEMAPPING_HPP
#define NATIVE_JNI_SDK_MESSAGEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct MessageMapping {
    static dxfg_message_t *toMessage(ByteReader &reader);
    static void fromMessage(dxfg_message_t *eventType, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_MESSAGEMAPPING_HPP
