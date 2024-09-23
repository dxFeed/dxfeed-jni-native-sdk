// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_OPTIONSALEMAPPING_HPP
#define NATIVE_JNI_SDK_OPTIONSALEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
struct ByteReader;
struct ByteWriter;

struct OptionSaleMapping {
    static dxfg_option_sale_t *toOptionSale(ByteReader &reader);
    static void fromOptionSale(dxfg_option_sale_t *optionSale, ByteWriter &writer);
};
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_OPTIONSALEMAPPING_HPP
