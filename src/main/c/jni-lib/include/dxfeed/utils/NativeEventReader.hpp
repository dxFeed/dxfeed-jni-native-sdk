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
    static dxfg_summary_t* toSummary(const char** pByteData, const double** pDoubleData);
    static dxfg_greeks_t* toGreeks(const char** pByteData, const double** pDoubleData);
    static dxfg_underlying_t* toUnderlying(const char** pByteData, const double** pDoubleData);
    static dxfg_theo_price_t* toTheoPrice(const char** pByteData, const double** pDoubleData);
    static dxfg_configuration_t* toConfiguration(const char** pByteData);
    static dxfg_message_t* toMessage(const char** pByteData);
    static dxfg_option_sale_t* toOptionSale(const char** pByteData, const double** pDoubleData);
    static dxfg_order_base_t* toOrderBase(const char** pByteData, const double** pDoubleData);
    static dxfg_order_t* toOrder(const char** pByteData, const double** pDoubleData);
    static dxfg_analytic_order_t* toAnalyticsOrder(const char** pByteData, const double** pDoubleData);
    static dxfg_spread_order_t* toSpreadOrder(const char** pByteData, const double** pDoubleData);

    static void readOrder(const char** pByteData, const double** pDoubleData, dxfg_order_base_t* orderBase);
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_TIMEANDSALEMAPPER_H_
