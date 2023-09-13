// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/NativeEventWriter.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  NativeEventWriter::NativeEventWriter() {
    eventTypes_.reserve(128);
    byteData_.reserve(128);
    doubleData_.reserve(128);
  }

  NativeEventWriter::~NativeEventWriter() {
    eventTypes_.clear();
    byteData_.clear();
    doubleData_.clear();
  }

  void NativeEventWriter::writeBytes(const int8_t* bytes, int32_t len) {
    // http://thewayofc.blogspot.com/2014/05/copying-memory-from-c-to-c-using.html
    byteData_.insert(byteData_.end(), bytes, bytes + len);
  }

  void NativeEventWriter::writeInt16_t(int16_t value) {
    byteData_.push_back(value & 0x00FF);
    byteData_.push_back((value & 0xFF00) >> 8);
  }

  void NativeEventWriter::writeInt32_t(int32_t value) {
    for (size_t i = 0; i < sizeof(int32_t); ++i) {
      byteData_.push_back(value & 0xFF);
      value >>= 8;
    }
  }

  void NativeEventWriter::writeInt64_t(int64_t value) {
    for (size_t i = 0; i < sizeof(int64_t); ++i) {
      byteData_.push_back(value & 0xFF);
      value >>= 8;
    }
  }

  void NativeEventWriter::writeDouble(double value) {
    doubleData_.push_back(value);
  }

  void NativeEventWriter::writeString(const char* str) {
    auto strSize = static_cast<int16_t>(strlen(str));
    writeInt16_t(strSize);
    static_assert(sizeof(int8_t) == sizeof(char));
    writeBytes(r_cast<const int8_t*>(str), strSize);
  }

  void NativeEventWriter::writeEvent(dxfg_event_type_t* eventType) {
    dxfg_event_clazz_t dxfgEventClazz = eventType->clazz;
    eventTypes_.push_back(dxfgEventClazz);
    switch (dxfgEventClazz) {
      case DXFG_EVENT_QUOTE: {
        fromQuote(r_cast<dxfg_quote_t*>(eventType));
      }
      case DXFG_EVENT_PROFILE: {
        fromProfile(r_cast<dxfg_profile_t*>(eventType));
      }
      case DXFG_EVENT_SUMMARY: {
        fromSummary(r_cast<dxfg_summary_t*>(eventType));
      }
      case DXFG_EVENT_GREEKS: {
        fromGreeks(r_cast<dxfg_greeks_t*>(eventType));
      }
      case DXFG_EVENT_CANDLE: {
        fromCandle(r_cast<dxfg_candle_t*>(eventType));
      }
      case DXFG_EVENT_UNDERLYING: {
        fromUnderlying(r_cast<dxfg_underlying_t*>(eventType));
      }
      case DXFG_EVENT_THEO_PRICE: {
        fromTheoPrice(r_cast<dxfg_theo_price_t*>(eventType));
      }
      case DXFG_EVENT_TRADE:
      case DXFG_EVENT_TRADE_ETH: {
        fromTrade(r_cast<dxfg_trade_base_t*>(eventType));
      }
      case DXFG_EVENT_TIME_AND_SALE: {
        fromTimeAndSale(r_cast<dxfg_time_and_sale_t*>(eventType));
      }
    }
  }

  jbyteArray NativeEventWriter::eventTypes(JNIEnv* env) {
    auto size = static_cast<int32_t>(eventTypes_.size());
    auto pArray = env->NewByteArray(size);
    auto data = r_cast<char*>(env->GetPrimitiveArrayCritical(pArray, 0));
    memcpy(data, eventTypes_.data(), size);
    env->ReleasePrimitiveArrayCritical(pArray, data, 0);
    return pArray;
  }

  jbyteArray NativeEventWriter::byteData(JNIEnv* env) {
    auto size = static_cast<int32_t>(eventTypes_.size());
    auto pArray = env->NewByteArray(size);
    auto data = r_cast<char*>(env->GetPrimitiveArrayCritical(pArray, 0));
    memcpy(data, byteData_.data(), size);
    env->ReleasePrimitiveArrayCritical(pArray, data, 0);
    return pArray;
  }

  jdoubleArray NativeEventWriter::doubleData(JNIEnv* env) {
    auto size = static_cast<int32_t>(eventTypes_.size());
    auto pArray = env->NewDoubleArray(size);
    auto data = r_cast<double*>(env->GetPrimitiveArrayCritical(pArray, 0));
    memcpy(data, doubleData_.data(), size);
    env->ReleasePrimitiveArrayCritical(pArray, data, 0);
    return pArray;
  }

  void NativeEventWriter::fromQuote(dxfg_quote_t* quote) {
    writeString(quote->market_event.event_symbol);
    writeInt64_t(quote->market_event.event_time);
    writeInt32_t(quote->time_millis_sequence);
    writeInt32_t(quote->time_nano_part);
    writeInt64_t(quote->bid_time);
    writeInt16_t(quote->bid_exchange_code);
    writeInt64_t(quote->ask_time);
    writeInt16_t(quote->ask_exchange_code);

    writeDouble(quote->bid_price);
    writeDouble(quote->bid_size);
    writeDouble(quote->ask_price);
    writeDouble(quote->ask_size);
  }

  void NativeEventWriter::fromProfile(dxfg_profile_t* profile) {
    writeString(profile->market_event.event_symbol);
    writeInt64_t(profile->market_event.event_time);

    writeInt64_t(profile->halt_start_time);
    writeInt64_t(profile->halt_end_time);
    writeInt32_t(profile->ex_dividend_day_id);
    writeInt32_t(profile->flags);
    writeString(profile->description);
    writeString(profile->status_reason);

    writeDouble(profile->high_limit_price);
    writeDouble(profile->low_limit_price);
    writeDouble(profile->high_52_week_price);
    writeDouble(profile->low_52_week_price);
    writeDouble(profile->beta);
    writeDouble(profile->earnings_per_share);
    writeDouble(profile->dividend_frequency);
    writeDouble(profile->ex_dividend_amount);
    writeDouble(profile->shares);
    writeDouble(profile->free_float);
  }

  void NativeEventWriter::fromSummary(dxfg_summary_t* summary) {
    writeString(summary->market_event.event_symbol);
    writeInt64_t(summary->market_event.event_time);
    writeInt32_t(summary->day_id);
    writeInt32_t(summary->prev_day_id);
    writeInt64_t(summary->open_interest);
    writeInt32_t(summary->flags);

    writeDouble(summary->day_open_price);
    writeDouble(summary->day_high_price);
    writeDouble(summary->day_low_price);
    writeDouble(summary->day_close_price);
    writeDouble(summary->prev_day_close_price);
    writeDouble(summary->prev_day_volume);
  }

  void NativeEventWriter::fromGreeks(dxfg_greeks_t* greeks) {
    writeString(greeks->market_event.event_symbol);
    writeInt64_t(greeks->market_event.event_time);
    writeInt32_t(greeks->event_flags);
    writeInt64_t(greeks->index);

    writeDouble(greeks->price);
    writeDouble(greeks->volatility);
    writeDouble(greeks->delta);
    writeDouble(greeks->gamma);
    writeDouble(greeks->theta);
    writeDouble(greeks->rho);
    writeDouble(greeks->vega);
  }

  void NativeEventWriter::fromCandle(dxfg_candle_t* candle) {
    writeString(candle->event_symbol);
    writeInt64_t(candle->event_time);
    writeInt32_t(candle->event_flags);
    writeInt64_t(candle->index);
    writeInt64_t(candle->count);

    writeDouble(candle->open);
    writeDouble(candle->high);
    writeDouble(candle->low);
    writeDouble(candle->close);
    writeDouble(candle->volume);
    writeDouble(candle->vwap);
    writeDouble(candle->bid_volume);
    writeDouble(candle->ask_volume);
    writeDouble(candle->imp_volatility);
    writeDouble(candle->open_interest);
  }

  void NativeEventWriter::fromUnderlying(dxfg_underlying_t* underlying) {
    writeString(underlying->market_event.event_symbol);
    writeInt64_t(underlying->market_event.event_time);
    writeInt32_t(underlying->event_flags);
    writeInt64_t(underlying->index);

    writeDouble(underlying->volatility);
    writeDouble(underlying->front_volatility);
    writeDouble(underlying->back_volatility);
    writeDouble(underlying->call_volume);
    writeDouble(underlying->put_volume);
    writeDouble(underlying->put_call_ratio);
  }

  void NativeEventWriter::fromTheoPrice(dxfg_theo_price_t* theoPrice) {
    writeString(theoPrice->market_event.event_symbol);
    writeInt64_t(theoPrice->market_event.event_time);
    writeInt32_t(theoPrice->event_flags);
    writeInt64_t(theoPrice->index);

    writeDouble(theoPrice->price);
    writeDouble(theoPrice->underlying_price);
    writeDouble(theoPrice->delta);
    writeDouble(theoPrice->gamma);
    writeDouble(theoPrice->dividend);
    writeDouble(theoPrice->interest);
  }

  void NativeEventWriter::fromTrade(dxfg_trade_base_t* trade) {
    writeString(trade->market_event.event_symbol);
    writeInt64_t(trade->market_event.event_time);
    writeInt64_t(trade->time_sequence);
    writeInt32_t(trade->time_nano_part);
    writeInt16_t(trade->exchange_code);
    writeInt32_t(trade->day_id);
    writeInt32_t(trade->flags);

    writeDouble(trade->price);
    writeDouble(trade->change);
    writeDouble(trade->size);
    writeDouble(trade->day_volume);
    writeDouble(trade->day_turnover);
  }

  void NativeEventWriter::fromTimeAndSale(dxfg_time_and_sale_t* tns) {
    writeString(tns->market_event.event_symbol);
    writeInt64_t(tns->market_event.event_time);
    writeInt32_t(tns->event_flags);
    writeInt64_t(tns->index);
    writeInt64_t(tns->time_nano_part);
    writeInt16_t(tns->exchange_code);
    writeInt32_t(tns->flags);

    writeString(tns->exchange_sale_conditions);
    writeString(tns->buyer);
    writeString(tns->seller);

    writeDouble(tns->price);
    writeDouble(tns->size);
    writeDouble(tns->bid_price);
    writeDouble(tns->ask_price);
  }
}