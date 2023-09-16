// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_TIMEANDSALEMAPPING_H
#define NATIVE_JNI_SDK_TIMEANDSALEMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct TimeAndSaleMapping {
    static dxfg_time_and_sale_t* toTimeAndSale(const char** pByteData, const double** pDoubleData);
    static void fromTimeAndSale(dxfg_time_and_sale_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_TIMEANDSALEMAPPING_H
