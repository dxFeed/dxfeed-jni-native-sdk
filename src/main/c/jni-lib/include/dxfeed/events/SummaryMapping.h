// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_SUMMARYMAPPING_H
#define NATIVE_JNI_SDK_SUMMARYMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct SummaryMapping {
    static dxfg_summary_t* toSummary(const char** pByteData, const double** pDoubleData);
    static void fromSummary(dxfg_summary_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_SUMMARYMAPPING_H
