// SPDX-License-Identifier: MPL-2.0


#include "dxfeed/utils/NativeEventReader.hpp"

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

  std::vector<dxfg_event_type_t*> NativeEventReader::toEvents(int size, char* pByteData, double* pDoubleData,
                                                              char* pEventTypes)
  {
    std::vector<dxfg_event_type_t*> events(size);
    for (int i = 0 ; i < size; ++i) {
      auto eventType = static_cast<dxfg_event_clazz_t>(readByte(&pEventTypes));
      switch (eventType) {
        case DXFG_EVENT_TIME_AND_SALE:
          events[i] = reinterpret_cast<dxfg_event_type_t*>(toTimeAndSale(pByteData, pDoubleData));
          break;
        case DXFG_EVENT_QUOTE:
          events[i] = reinterpret_cast<dxfg_event_type_t*>(toQuote(pByteData, pDoubleData));
          break;
        case DXFG_EVENT_CANDLE:
          events[i] = reinterpret_cast<dxfg_event_type_t*>(toCandle(pByteData, pDoubleData));
          break;
      }
    }
    return events;
  }

  dxfg_event_type_t* NativeEventReader::toEvent(char* pByteData, double* pDoubleData, dxfg_event_clazz_t eventType) {
    switch (eventType) {
      case DXFG_EVENT_TIME_AND_SALE:
        return reinterpret_cast<dxfg_event_type_t*>(toTimeAndSale(pByteData, pDoubleData));
      case DXFG_EVENT_QUOTE:
        return reinterpret_cast<dxfg_event_type_t*>(toQuote(pByteData, pDoubleData));
      case DXFG_EVENT_CANDLE:
        return reinterpret_cast<dxfg_event_type_t*>(toCandle(pByteData, pDoubleData));
      default: {
        return nullptr;
      }
    }
  }

  dxfg_time_and_sale_t* NativeEventReader::toTimeAndSale(char* pByteData, double* pDoubleData) {
    auto* quote = new dxfg_time_and_sale_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_TIME_AND_SALE;
    int16_t strSize = readInt16_t(&pByteData);
    quote->market_event.event_symbol = pByteData;
    pByteData += strSize;
    quote->market_event.event_time = readLong(&pByteData);
    quote->event_flags = readInt(&pByteData);
    quote->index = readLong(&pByteData);
    quote->time_nano_part = readInt(&pByteData);
    quote->exchange_code = readInt16_t(&pByteData);
    quote->size = static_cast<double>(readLong(&pByteData));
    quote->flags = readInt(&pByteData);

    strSize = readInt16_t(&pByteData);
    quote->exchange_sale_conditions = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote->buyer = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote->seller = strSize ? pByteData : "";
    pByteData += strSize;

    quote->price = readDouble(&pDoubleData);
    quote->bid_price = readDouble(&pDoubleData);
    quote->ask_price = readDouble(&pDoubleData);
    return quote;
  }

  dxfg_quote_t* NativeEventReader::toQuote(char* pByteData, double* pDoubleData) {
    auto* quote = new dxfg_quote_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
    int16_t strSize = readInt16_t(&pByteData);
    quote->market_event.event_symbol = pByteData;
    pByteData += strSize;
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
    int16_t strSize = readInt16_t(&pByteData);
    quote->event_symbol = pByteData;
    pByteData += strSize;
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
}