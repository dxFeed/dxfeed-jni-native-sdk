// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_
#define DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_

#include <vector>

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventReader {
    static std::vector<dxfg_event_type_t*> toEvents(int size, const char* pByteData, const double* pDoubleData, const char* pEventTypes);
    static dxfg_event_type_t* toEvent(const char** pByteData, const double** pDoubleData, dxfg_event_clazz_t pEventTypes);
  private:
    static dxfg_time_and_sale_t* toTimeAndSale(const char** pByteData, const double** pDoubleData);
    static dxfg_quote_t* toQuote(const char** pByteData, const double** pDoubleData);
    static dxfg_candle_t* toCandle(const char** pByteData, const double** pDoubleData);
    static dxfg_trade_t* toTrade(const char** pByteData, const double** pDoubleData);
    static dxfg_profile_t* toProfile(const char** pByteData, const double** pDoubleData);
    static dxfg_underlying_t* toUnderlying(const char** pByteData, const double** pDoubleData);
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_
