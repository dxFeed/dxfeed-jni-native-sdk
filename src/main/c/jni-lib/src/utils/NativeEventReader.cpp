// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/utils/NativeEventReader.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  template <typename T>
  inline T readUByte(const char** pData) {
    T val = (**pData) & 0xFF;
    ++(*pData);
    return val;
  }

  template <typename PrimitiveT>
  inline PrimitiveT readPrimitive(const char** pData) {
    PrimitiveT value = 0;
    for (size_t n = 0; n < sizeof(PrimitiveT); ++n) {
      value |= readUByte<PrimitiveT>(pData) << (n * 8);
    }
    return value;
  }

  inline int8_t readByte(const char** pData) {
    return readUByte<int8_t>(pData);
  }

  inline int16_t readInt16_t(const char** pData) {
    return readPrimitive<int16_t>(pData);
  }

  inline int32_t readInt(const char** pData) {
    return readPrimitive<int32_t>(pData);
  }

  inline int64_t readLong(const char** pData) {
    return readPrimitive<int64_t>(pData);
  }

  inline double readDouble(const double** pData) {
    double value = **pData;
    ++(*pData);
    return value;
  }

  inline const char* readString(const char** pData) {
    int16_t strSize = readInt16_t(pData);
    const auto result = strSize ? *pData : "";
    (*pData) += strSize;
    return result;
  }

  std::vector<dxfg_event_type_t*> NativeEventReader::toEvents(int size, const char* pByteData, const double* pDoubleData,
                                                              const char* pEventTypes)
  {
    std::vector<dxfg_event_type_t*> events(size);
    for (int i = 0 ; i < size; ++i) {
//      std::cout << "events[i] = " << i << ", pByteData = 0x" << (void*)pByteData << ", pDoubleData = 0x" << (void*)pDoubleData << std::endl;
      auto eventType = static_cast<dxfg_event_clazz_t>(readByte(&pEventTypes));
      events[i] = toEvent(&pByteData, &pDoubleData, eventType);
    }
    return events;
  }

  dxfg_event_type_t* NativeEventReader::toEvent(const char** pByteData, const double** pDoubleData, dxfg_event_clazz_t eventType) {
    switch (eventType) {
      case DXFG_EVENT_TIME_AND_SALE:
        return dxfeed::r_cast<dxfg_event_type_t*>(toTimeAndSale(pByteData, pDoubleData));
      case DXFG_EVENT_QUOTE:
        return dxfeed::r_cast<dxfg_event_type_t*>(toQuote(pByteData, pDoubleData));
      case DXFG_EVENT_CANDLE:
        return dxfeed::r_cast<dxfg_event_type_t*>(toCandle(pByteData, pDoubleData));
      case DXFG_EVENT_TRADE:
        return dxfeed::r_cast<dxfg_event_type_t*>(toTrade(pByteData, pDoubleData));
      case DXFG_EVENT_PROFILE:
        return dxfeed::r_cast<dxfg_event_type_t*>(toProfile(pByteData, pDoubleData));
      default: {
        std::cout << "NativeEventReader::toEvent = " << nullptr << std::endl;
        return nullptr;
      }
    }
  }

  dxfg_time_and_sale_t* NativeEventReader::toTimeAndSale(const char** pByteData, const double** pDoubleData) {
    auto* tns = new dxfg_time_and_sale_t();
    tns->market_event.event_type.clazz = DXFG_EVENT_TIME_AND_SALE;
    tns->market_event.event_symbol = readString(pByteData);
    tns->market_event.event_time = readLong(pByteData);
    tns->event_flags = readInt(pByteData);
    tns->index = readLong(pByteData);
    tns->time_nano_part = readInt(pByteData);
    tns->exchange_code = readInt16_t(pByteData);
    tns->size = static_cast<double>(readLong(pByteData));
    tns->flags = readInt(pByteData);

    tns->exchange_sale_conditions = readString(pByteData);
    tns->buyer = readString(pByteData);
    tns->seller = readString(pByteData);

    tns->price = readDouble(pDoubleData);
    tns->bid_price = readDouble(pDoubleData);
    tns->ask_price = readDouble(pDoubleData);
    return tns;
  }

  dxfg_quote_t* NativeEventReader::toQuote(const char** pByteData, const double** pDoubleData) {
    auto* quote = new dxfg_quote_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
    quote->market_event.event_symbol = readString(pByteData);
    quote->market_event.event_time = readLong(pByteData);
    quote->time_millis_sequence = readInt(pByteData);
    quote->time_nano_part = readInt(pByteData);
    quote->bid_time = readLong(pByteData);
    quote->bid_exchange_code = readInt16_t(pByteData);
    quote->ask_time = readLong(pByteData);
    quote->ask_exchange_code = readInt16_t(pByteData);

    quote->bid_price = readDouble(pDoubleData);
    quote->bid_size = readDouble(pDoubleData);
    quote->ask_price = readDouble(pDoubleData);
    quote->ask_size = readDouble(pDoubleData);
    return quote;
  }

  dxfg_candle_t* NativeEventReader::toCandle(const char** pByteData, const double** pDoubleData) {
    auto* candle = new dxfg_candle_t();
    candle->event_type.clazz = DXFG_EVENT_CANDLE;
    candle->event_symbol = readString(pByteData);
    candle->event_time = readLong(pByteData);
    candle->event_flags = readInt(pByteData);
    candle->index = readLong(pByteData);
    candle->count = readLong(pByteData);
    candle->volume = static_cast<double>(readLong(pByteData));
    candle->bid_volume = static_cast<double>(readLong(pByteData));
    candle->ask_volume = static_cast<double>(readLong(pByteData));
    candle->open_interest = static_cast<double>(readLong(pByteData));

    candle->open = readDouble(pDoubleData);
    candle->high = readDouble(pDoubleData);
    candle->low = readDouble(pDoubleData);
    candle->close = readDouble(pDoubleData);
    candle->vwap = readDouble(pDoubleData);
    candle->imp_volatility = readDouble(pDoubleData);
    return candle;
  }

  dxfg_trade_t* NativeEventReader::toTrade(const char** pByteData, const double** pDoubleData) {
    auto* trade = new dxfg_trade_base_t();
    trade->market_event.event_type.clazz = DXFG_EVENT_TRADE;
    trade->market_event.event_symbol = readString(pByteData);
    trade->market_event.event_time = readLong(pByteData);
    trade->time_sequence = readLong(pByteData);
    trade->time_nano_part = readInt(pByteData);
    trade->exchange_code = readInt16_t(pByteData);
    trade->day_id = readInt(pByteData);
    trade->flags = readInt(pByteData);

    trade->price = readDouble(pDoubleData);
    trade->change = readDouble(pDoubleData);
    trade->size = readDouble(pDoubleData);
    trade->day_volume = readDouble(pDoubleData);
    trade->day_turnover = readDouble(pDoubleData);
    return r_cast<dxfg_trade_t*>(trade);
  }

  dxfg_profile_t* NativeEventReader::toProfile(const char** pByteData, const double** pDoubleData) {
    auto* profile = new dxfg_profile_t();
    profile->market_event.event_type.clazz = DXFG_EVENT_PROFILE;
    profile->market_event.event_symbol = readString(pByteData);
    profile->market_event.event_time = readLong(pByteData);

    profile->halt_start_time = readLong(pByteData);
    profile->halt_end_time = readLong(pByteData);
    profile->ex_dividend_day_id = readInt(pByteData);
    profile->flags = readInt(pByteData);
    profile->description = readString(pByteData);
    profile->status_reason = readString(pByteData);

    profile->high_limit_price = readDouble(pDoubleData);
    profile->low_limit_price = readDouble(pDoubleData);
    profile->high_52_week_price = readDouble(pDoubleData);
    profile->low_52_week_price = readDouble(pDoubleData);
    profile->beta = readDouble(pDoubleData);
    profile->earnings_per_share = readDouble(pDoubleData);
    profile->dividend_frequency = readDouble(pDoubleData);
    profile->ex_dividend_amount = readDouble(pDoubleData);
    profile->shares = readDouble(pDoubleData);
    profile->free_float = readDouble(pDoubleData);
    return profile;
  }
}