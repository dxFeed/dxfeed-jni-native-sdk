// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_TIMEANDSALEMAPPING_HPP
#define NATIVE_JNI_SDK_TIMEANDSALEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct TimeAndSaleMapping {
    static dxfg_time_and_sale_t* toTimeAndSale(ByteReader& reader);
    static void fromTimeAndSale(dxfg_time_and_sale_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_TIMEANDSALEMAPPING_HPP
