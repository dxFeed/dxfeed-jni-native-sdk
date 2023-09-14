// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_NATIVEEVENTWRITER_H_
#define DXFEED_JNI_NATIVE_SDK_NATIVEEVENTWRITER_H_

#include <vector>

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventWriter {
    using vector_uint8_t = std::vector<uint8_t>;
    using vector_double = std::vector<double>;

    explicit NativeEventWriter();
    ~NativeEventWriter();
    void writeEvent(dxfg_event_type_t* eventType);

    jbyteArray eventTypes(JNIEnv* env);
    jbyteArray byteData(JNIEnv* env);
    jdoubleArray doubleData(JNIEnv* env);
  private:
    void fromQuote(dxfg_quote_t* eventType);
    void fromProfile(dxfg_profile_t* eventType);
    void fromSummary(dxfg_summary_t* eventType);
    void fromGreeks(dxfg_greeks_t* eventType);
    void fromCandle(dxfg_candle_t* eventType);
    void fromUnderlying(dxfg_underlying_t* eventType);
    void fromTheoPrice(dxfg_theo_price_t* theo);
    void fromTrade(dxfg_trade_base_t* eventType);
    void fromTimeAndSale(dxfg_time_and_sale_t* eventType);
    void fromOrderBase(dxfg_order_base_t* eventType);
    void fromOrder(dxfg_order_t* eventType);
    void fromAnalyticOrder(dxfg_analytic_order_t* eventType);
    void fromSpreadOrder(dxfg_spread_order_t* eventType);

    inline void writeBytes(const int8_t* bytes, int32_t len);
    inline void writeString(const char* str);
    inline void writeInt16_t(int16_t value);
    inline void writeInt32_t(int32_t value);
    inline void writeInt64_t(int64_t value);
    inline void writeDouble(double value);

    std::vector<uint8_t> eventTypes_;
    std::vector<uint8_t> byteData_;
    std::vector<double> doubleData_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_NATIVEEVENTWRITER_H_
