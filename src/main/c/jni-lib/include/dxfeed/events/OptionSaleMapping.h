// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_OPTIONSALEMAPPING_H
#define NATIVE_JNI_SDK_OPTIONSALEMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct OptionSaleMapping {
    static dxfg_option_sale_t* toOptionSale(const char** pByteData, const double** pDoubleData);
    static void fromOptionSale(dxfg_option_sale_t* optionSale, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_OPTIONSALEMAPPING_H
