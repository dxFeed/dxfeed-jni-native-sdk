// SPDX-License-Identifier: MPL-2.0

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
        return r_cast<dxfg_event_type_t*>(toTimeAndSale(pByteData, pDoubleData));
      case DXFG_EVENT_QUOTE:
        return r_cast<dxfg_event_type_t*>(toQuote(pByteData, pDoubleData));
      case DXFG_EVENT_CANDLE:
        return r_cast<dxfg_event_type_t*>(toCandle(pByteData, pDoubleData));
      case DXFG_EVENT_TRADE:
        return r_cast<dxfg_event_type_t*>(toTrade(pByteData, pDoubleData));
      case DXFG_EVENT_PROFILE:
        return r_cast<dxfg_event_type_t*>(toProfile(pByteData, pDoubleData));
      case DXFG_EVENT_UNDERLYING:
        return r_cast<dxfg_event_type_t*>(toUnderlying(pByteData, pDoubleData));
      case DXFG_EVENT_THEO_PRICE:
        return r_cast<dxfg_event_type_t*>(toTheoPrice(pByteData, pDoubleData));
      case DXFG_EVENT_CONFIGURATION:
        return r_cast<dxfg_event_type_t*>(toConfiguration(pByteData));
      case DXFG_EVENT_MESSAGE:
        return r_cast<dxfg_event_type_t*>(toMessage(pByteData));
      case DXFG_EVENT_OPTION_SALE:
        return r_cast<dxfg_event_type_t*>(toOptionSale(pByteData, pDoubleData));
      default: {
        javaLogger->info("NativeEventReader::toEvent = ", nullptr);
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
    tns->flags = readInt(pByteData);

    tns->exchange_sale_conditions = readString(pByteData);
    tns->buyer = readString(pByteData);
    tns->seller = readString(pByteData);

    tns->price = readDouble(pDoubleData);
    tns->size = readDouble(pDoubleData);
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

    candle->open = readDouble(pDoubleData);
    candle->high = readDouble(pDoubleData);
    candle->low = readDouble(pDoubleData);
    candle->close = readDouble(pDoubleData);
    candle->volume = readDouble(pDoubleData);
    candle->vwap = readDouble(pDoubleData);
    candle->bid_volume = readDouble(pDoubleData);
    candle->ask_volume = readDouble(pDoubleData);
    candle->imp_volatility = readDouble(pDoubleData);
    candle->open_interest = readDouble(pDoubleData);
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

  dxfg_underlying_t* NativeEventReader::toUnderlying(const char** pByteData, const double** pDoubleData) {
    auto* underlying = new dxfg_underlying_t();
    underlying->market_event.event_type.clazz = DXFG_EVENT_UNDERLYING;
    underlying->market_event.event_symbol = readString(pByteData);
    underlying->market_event.event_time = readLong(pByteData);

    underlying->event_flags = readInt(pByteData);
    underlying->index = readLong(pByteData);

    underlying->volatility = readDouble(pDoubleData);
    underlying->front_volatility = readDouble(pDoubleData);
    underlying->back_volatility = readDouble(pDoubleData);
    underlying->call_volume = readDouble(pDoubleData);
    underlying->put_volume = readDouble(pDoubleData);
    underlying->put_call_ratio = readDouble(pDoubleData);

    return underlying;
  }

  dxfg_theo_price_t* NativeEventReader::toTheoPrice(const char** pByteData, const double** pDoubleData) {
    auto* theoPrice = new dxfg_theo_price_t();
    theoPrice->market_event.event_type.clazz = DXFG_EVENT_UNDERLYING;
    theoPrice->market_event.event_symbol = readString(pByteData);
    theoPrice->market_event.event_time = readLong(pByteData);

    theoPrice->event_flags = readInt(pByteData);
    theoPrice->index = readLong(pByteData);

    theoPrice->price = readDouble(pDoubleData);
    theoPrice->underlying_price = readDouble(pDoubleData);
    theoPrice->delta = readDouble(pDoubleData);
    theoPrice->gamma = readDouble(pDoubleData);
    theoPrice->dividend = readDouble(pDoubleData);
    theoPrice->interest = readDouble(pDoubleData);

    return theoPrice;
  }

  dxfg_configuration_t* NativeEventReader::toConfiguration(const char** pByteData) {
    auto* configuration = new dxfg_configuration_t();// todo: make wrapper to get attachment fromJava
    configuration->event_type.clazz = DXFG_EVENT_CONFIGURATION;
    configuration->event_symbol = readString(pByteData);
    configuration->event_time = readLong(pByteData);
    configuration->version = readInt(pByteData);
    // todo: read next ID of type LONG, ID points to java.lang.Object attachment in Java
    // configuration->attachment = readBlob();
    return configuration;
  }

  dxfg_message_t* NativeEventReader::toMessage(const char** pByteData) {
    auto* message = new dxfg_message_t(); // todo: make wrapper to get attachment from Java
    message->event_type.clazz = DXFG_EVENT_MESSAGE;
    message->event_symbol = readString(pByteData);
    message->event_time = readLong(pByteData);
    // todo: read next ID of type LONG, ID points to java.lang.Object attachment in Java
    // message->attachment = readBlob(pByteData);
    return message;
  }

  dxfg_option_sale_t* NativeEventReader::toOptionSale(const char** pByteData, const double** pDoubleData) {
    auto* optionSale = new dxfg_option_sale_t();
    optionSale->market_event.event_type.clazz = DXFG_EVENT_OPTION_SALE;
    optionSale->market_event.event_symbol = readString(pByteData);
    optionSale->market_event.event_time = readLong(pByteData);
    optionSale->event_flags = readInt(pByteData);
    optionSale->index = readLong(pByteData);
    optionSale->time_sequence = readLong(pByteData);
    optionSale->time_nano_part = readInt(pByteData);
    optionSale->exchange_code = readInt16_t(pByteData);
    optionSale->flags = readInt(pByteData);

    optionSale->exchange_sale_conditions = readString(pByteData);
    optionSale->option_symbol = readString(pByteData);

    optionSale->price = readDouble(pDoubleData);
    optionSale->size = readDouble(pDoubleData);
    optionSale->bid_price = readDouble(pDoubleData);
    optionSale->ask_price = readDouble(pDoubleData);
    optionSale->underlying_price = readDouble(pDoubleData);
    optionSale->volatility = readDouble(pDoubleData);
    optionSale->delta = readDouble(pDoubleData);
    return optionSale;
  }

}