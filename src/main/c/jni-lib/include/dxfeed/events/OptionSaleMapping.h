// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_OPTIONSALEMAPPING_H
#define NATIVE_JNI_SDK_OPTIONSALEMAPPING_H

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct OptionSaleMapping {
    static dxfg_option_sale_t* toOptionSale(ByteReader& reader);
    static void fromOptionSale(dxfg_option_sale_t* optionSale, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_OPTIONSALEMAPPING_H