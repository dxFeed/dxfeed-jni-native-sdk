// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_QUOTEMAPPING_H
#define NATIVE_JNI_SDK_QUOTEMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/NativeEventWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct QuoteMapping {
    static dxfg_quote_t* toQuote(const char** pByteData, const double** pDoubleData);
    static void fromQuote(dxfg_quote_t* eventType, NativeEventWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_QUOTEMAPPING_H
