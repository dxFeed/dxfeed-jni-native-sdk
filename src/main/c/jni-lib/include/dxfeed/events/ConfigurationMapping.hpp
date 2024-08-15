// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_CONFIGURATIONMAPPING_HPP
#define NATIVE_JNI_SDK_CONFIGURATIONMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct ConfigurationMapping {
    static dxfg_configuration_t *toConfiguration(ByteReader &reader);
    static void fromConfiguration(dxfg_configuration_t *eventType, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_CONFIGURATIONMAPPING_HPP
