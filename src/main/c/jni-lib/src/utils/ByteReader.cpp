// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/events/CandleMapping.h"
#include "dxfeed/events/ConfigurationMapping.h"
#include "dxfeed/events/GreeksMapping.h"
#include "dxfeed/events/MessageMapping.h"
#include "dxfeed/events/OptionSaleMapping.h"
#include "dxfeed/events/OrderMapping.h"
#include "dxfeed/events/ProfileMapping.h"
#include "dxfeed/events/QuoteMapping.h"
#include "dxfeed/events/SeriesMapping.h"
#include "dxfeed/events/SummaryMapping.h"
#include "dxfeed/events/TheoPriceMapping.h"
#include "dxfeed/events/TimeAndSaleMapping.h"
#include "dxfeed/events/TradeMapping.h"
#include "dxfeed/events/UnderlyingMapping.h"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  ByteReader::ByteReader(int size, const char* pByteData, const double* pDoubleData, const char* pEventTypes):
    size_(size),
    pByteData_(pByteData),
    pDoubleData_(pDoubleData),
    pEventTypes_(pEventTypes)
  {}

  dxfg_event_clazz_t ByteReader::readEventType() {
    uint8_t val = *pEventTypes_;
    ++(pEventTypes_);
    return static_cast<dxfg_event_clazz_t>(val);
  }

  uint8_t ByteReader::readUByte() {
    uint8_t val = (*pByteData_) & 0xFF;
    ++(pByteData_);
    return val;
  }

  int16_t ByteReader::readInt16_t() {
    return readPrimitive<int16_t>();
  }

  int32_t ByteReader::readInt() {
    return readPrimitive<int32_t>();
  }

  int64_t ByteReader::readLong() {
    return readPrimitive<int64_t>();
  }

  double ByteReader::readDouble() {
    double value = *pDoubleData_;
    ++(pDoubleData_);
    return value;
  }

  const char* ByteReader::readString() {
    int16_t strSize = readInt16_t();
    const auto result = strSize ? pByteData_ : nullptr;
    pByteData_ += strSize;
    return result;
  }

  std::vector<dxfg_event_type_t*> ByteReader::toEvents() {
    std::vector<dxfg_event_type_t*> events(size_);
    for (int i = 0 ; i < size_; ++i) {
      events[i] = toEvent(readEventType());
    }
    return events;
  }

  dxfg_event_type_t* ByteReader::toEvent(dxfg_event_clazz_t eventType) {
    switch (eventType) {
      case DXFG_EVENT_QUOTE: {
        return r_cast<dxfg_event_type_t*>(QuoteMapping::toQuote(*this));
      }
      case DXFG_EVENT_PROFILE: {
        return r_cast<dxfg_event_type_t*>(ProfileMapping::toProfile(*this));
      }
      case DXFG_EVENT_SUMMARY: {
        return r_cast<dxfg_event_type_t*>(SummaryMapping::toSummary(*this));
      }
      case DXFG_EVENT_GREEKS: {
        return r_cast<dxfg_event_type_t*>(GreeksMapping::toGreeks(*this));
      }
      case DXFG_EVENT_CANDLE: {
        return r_cast<dxfg_event_type_t*>(CandleMapping::toCandle(*this));
      }
      case DXFG_EVENT_UNDERLYING: {
        return r_cast<dxfg_event_type_t*>(UnderlyingMapping::toUnderlying(*this));
      }
      case DXFG_EVENT_THEO_PRICE: {
        return r_cast<dxfg_event_type_t*>(TheoPriceMapping::toTheoPrice(*this));
      }
      case DXFG_EVENT_TRADE_ETH:
      case DXFG_EVENT_TRADE: {
        return r_cast<dxfg_event_type_t*>(TradeMapping::toTradeBase(*this));
      }
      case DXFG_EVENT_CONFIGURATION: {
        return r_cast<dxfg_event_type_t*>(ConfigurationMapping::toConfiguration(*this));
      }
      case DXFG_EVENT_MESSAGE: {
        return r_cast<dxfg_event_type_t*>(MessageMapping::toMessage(*this));
      }
      case DXFG_EVENT_TIME_AND_SALE: {
        return r_cast<dxfg_event_type_t*>(TimeAndSaleMapping::toTimeAndSale(*this));
      }
      case DXFG_EVENT_ORDER_BASE: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toOrderBase(*this));
      }
      case DXFG_EVENT_ORDER: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toOrder(*this));
      }
      case DXFG_EVENT_ANALYTIC_ORDER: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toAnalyticsOrder(*this));
      }
      case DXFG_EVENT_SPREAD_ORDER: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toSpreadOrder(*this));
      }
      case DXFG_EVENT_SERIES: {
        return r_cast<dxfg_event_type_t*>(SeriesMapping::toSeries(*this));
      }
      case DXFG_EVENT_OPTION_SALE: {
        return r_cast<dxfg_event_type_t*>(OptionSaleMapping::toOptionSale(*this));
      }
      default: {
        javaLogger->info("NativeEventReader::toEvent = ", nullptr);
        return nullptr;
      }
    }
  }
}