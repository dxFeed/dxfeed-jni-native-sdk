// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_
#define DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_

#include <vector>

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventReader {
    static std::vector<dxfg_event_type_t*> toEvents(int size, char* pByteData, double* pDoubleData, char* pEventTypes);
  private:
    static dxfg_time_and_sale_t* toTimeAndSale(char* pByteData, double* pDoubleData);
    static dxfg_quote_t* toQuote(char* pByteData, double* pDoubleData);
    static dxfg_candle_t* toCandle(char* pByteData, double* pDoubleData);
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_
