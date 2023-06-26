// SPDX-License-Identifier: MPL-2.0


#include "dxfeed/utils/NativeEventReader.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  template <typename T>
  inline T readUByte(char** pData) {
    T val = (**pData) & 0xFF;
    ++(*pData);
    return val;
  }

  template <typename PrimitiveT>
  inline PrimitiveT readPrimitive(char** pData) {
    PrimitiveT value = 0;
    for (size_t n = 0; n < sizeof(PrimitiveT); ++n) {
      value += readUByte<PrimitiveT>(pData) << (n * 8);
    }
    return value;
  }

  inline int8_t readByte(char** pData) {
    return readUByte<int8_t>(pData);
  }

  inline int16_t readInt16_t(char** pData) {
    return readPrimitive<int16_t>(pData);
  }

  inline int32_t readInt(char** pData) {
    return readPrimitive<int32_t>(pData);
  }

  inline int64_t readLong(char** pData) {
    return readPrimitive<int64_t>(pData);
  }

  inline double readDouble(double** pData) {
    double value = **pData;
    ++(*pData);
    return value;
  }

  inline const char* readString(char** pData) {
    int16_t strSize = readInt16_t(pData);
    const auto result = strSize ? *pData : "";
    (*pData) += strSize;
    return result;
  }

  std::vector<dxfg_event_type_t*> NativeEventReader::toEvents(int size, char* pByteData, double* pDoubleData,
                                                              char* pEventTypes)
  {
    std::vector<dxfg_event_type_t*> events(size);
    for (int i = 0 ; i < size; ++i) {
      auto eventType = static_cast<dxfg_event_clazz_t>(readByte(&pEventTypes));
      events[i] = toEvent(pByteData, pDoubleData, eventType);
    }
    return events;
  }

  dxfg_event_type_t* NativeEventReader::toEvent(char* pByteData, double* pDoubleData, dxfg_event_clazz_t eventType) {
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
        return nullptr;
      }
    }
  }

  dxfg_time_and_sale_t* NativeEventReader::toTimeAndSale(char* pByteData, double* pDoubleData) {
    auto* quote = new dxfg_time_and_sale_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_TIME_AND_SALE;
    quote->market_event.event_symbol = readString(&pByteData);
    quote->market_event.event_time = readLong(&pByteData);
    quote->event_flags = readInt(&pByteData);
    quote->index = readLong(&pByteData);
    quote->time_nano_part = readInt(&pByteData);
    quote->exchange_code = readInt16_t(&pByteData);
    quote->size = static_cast<double>(readLong(&pByteData));
    quote->flags = readInt(&pByteData);

    quote->exchange_sale_conditions = readString(&pByteData);
    quote->buyer = readString(&pByteData);
    quote->seller = readString(&pByteData);

    quote->price = readDouble(&pDoubleData);
    quote->bid_price = readDouble(&pDoubleData);
    quote->ask_price = readDouble(&pDoubleData);
    return quote;
  }

  dxfg_quote_t* NativeEventReader::toQuote(char* pByteData, double* pDoubleData) {
    auto* quote = new dxfg_quote_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
    quote->market_event.event_symbol = readString(&pByteData);
    quote->market_event.event_time = readLong(&pByteData);
    quote->time_millis_sequence = readInt(&pByteData);
    quote->time_nano_part = readInt(&pByteData);
    quote->bid_time = readLong(&pByteData);
    quote->bid_exchange_code = readInt16_t(&pByteData);
    quote->bid_size = static_cast<double>(readLong(&pByteData));
    quote->ask_time = readLong(&pByteData);
    quote->ask_exchange_code = readInt16_t(&pByteData);
    quote->ask_size = static_cast<double>(readLong(&pByteData));

    quote->bid_price = readDouble(&pDoubleData);
    quote->ask_price = readDouble(&pDoubleData);
    return quote;
  }

  dxfg_candle_t* NativeEventReader::toCandle(char* pByteData, double* pDoubleData) {
    auto* quote = new dxfg_candle_t();
    quote->event_type.clazz = DXFG_EVENT_CANDLE;
    quote->event_symbol = readString(&pByteData);
    quote->event_time = readLong(&pByteData);
    quote->event_flags = readInt(&pByteData);
    quote->index = readLong(&pByteData);
    quote->count = readLong(&pByteData);
    quote->volume = static_cast<double>(readLong(&pByteData));
    quote->bid_volume = static_cast<double>(readLong(&pByteData));
    quote->ask_volume = static_cast<double>(readLong(&pByteData));
    quote->open_interest = static_cast<double>(readLong(&pByteData));

    quote->open = readDouble(&pDoubleData);
    quote->high = readDouble(&pDoubleData);
    quote->low = readDouble(&pDoubleData);
    quote->close = readDouble(&pDoubleData);
    quote->vwap = readDouble(&pDoubleData);
    quote->imp_volatility = readDouble(&pDoubleData);
    return quote;
  }

  dxfg_trade_t* NativeEventReader::toTrade(char* pByteData, double* pDoubleData) {
    auto* trade = new dxfg_trade_t();
    trade->trade_base.market_event.event_type.clazz = DXFG_EVENT_TRADE;
    trade->trade_base.market_event.event_symbol = readString(&pByteData);
    trade->trade_base.market_event.event_time = readLong(&pByteData);

    trade->trade_base.time_sequence = readLong(&pByteData);
    trade->trade_base.time_nano_part = readInt(&pByteData);
    trade->trade_base.exchange_code = readInt16_t(&pByteData);
    trade->trade_base.day_id = readInt(&pByteData);
    trade->trade_base.flags = readInt(&pByteData);

    trade->trade_base.price = readDouble(&pDoubleData);
    trade->trade_base.change = readDouble(&pDoubleData);
    trade->trade_base.size = readDouble(&pDoubleData);
    trade->trade_base.day_volume = readDouble(&pDoubleData);
    trade->trade_base.day_turnover = readDouble(&pDoubleData);
    return trade;
  }

  dxfg_profile_t* NativeEventReader::toProfile(char* pByteData, double* pDoubleData) {
    auto* profile = new dxfg_profile_t();
    profile->market_event.event_type.clazz = DXFG_EVENT_PROFILE;
    profile->market_event.event_symbol = readString(&pByteData);
    profile->market_event.event_time = readLong(&pByteData);

    profile->halt_start_time = readLong(&pByteData);
    profile->halt_end_time = readLong(&pByteData);
    profile->ex_dividend_day_id = readInt(&pByteData);
    profile->flags = readInt(&pByteData);
    profile->description = readString(&pByteData);
    profile->status_reason = readString(&pByteData);

    profile->high_limit_price = readDouble(&pDoubleData);
    profile->low_limit_price = readDouble(&pDoubleData);
    profile->high_52_week_price = readDouble(&pDoubleData);
    profile->low_52_week_price = readDouble(&pDoubleData);
    profile->beta = readDouble(&pDoubleData);
    profile->earnings_per_share = readDouble(&pDoubleData);
    profile->dividend_frequency = readDouble(&pDoubleData);
    profile->ex_dividend_amount = readDouble(&pDoubleData);
    profile->shares = readDouble(&pDoubleData);
    profile->free_float = readDouble(&pDoubleData);
    return profile;
  }
}