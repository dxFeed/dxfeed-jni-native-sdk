// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_SERIESMAPPING_H
#define NATIVE_JNI_SDK_SERIESMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct SeriesMapping {
    static dxfg_series_t* toSeries(const char** pByteData, const double** pDoubleData);
    static void fromSeries(dxfg_series_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_SERIESMAPPING_H
