// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/CandleMapping.h"
#include "dxfeed/events/EventReader.h"
#include "dxfeed/events/GreeksMapping.h"
#include "dxfeed/events/OptionSaleMapping.h"
#include "dxfeed/events/OrderMapping.h"
#include "dxfeed/events/ProfileMapping.h"
#include "dxfeed/events/QuoteMapping.h"
#include "dxfeed/events/SeriesMapping.h"
#include "dxfeed/events/SummaryMapping.h"
#include "dxfeed/events/TheoPriceMapping.h"
#include "dxfeed/events/TradeMapping.h"
#include "dxfeed/events/TimeAndSaleMapping.h"
#include "dxfeed/events/UnderlyingMapping.h"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/NativeEventReader.hpp"

namespace dxfeed::jni {
  std::vector<dxfg_event_type_t*> NativeEventReader::toEvents(int size,
                                                              const char* pByteData,
                                                              const double* pDoubleData,
                                                              const char* pEventTypes)
  {
    std::vector<dxfg_event_type_t*> events(size);
    for (int i = 0 ; i < size; ++i) {
//      std::cout << "events[i] = " << i << ", pByteData = 0x" << (void*)pByteData << ", pDoubleData = 0x" << (void*)pDoubleData << std::endl;
      auto eventType = static_cast<dxfg_event_clazz_t>(EventReader::readByte(&pEventTypes));
      events[i] = toEvent(&pByteData, &pDoubleData, eventType);
    }
    return events;
  }

  dxfg_event_type_t* NativeEventReader::toEvent(const char** pByteData,
                                                const double** pDoubleData,
                                                dxfg_event_clazz_t eventType)
  {
    switch (eventType) {
      case DXFG_EVENT_QUOTE: {
        return r_cast<dxfg_event_type_t*>(QuoteMapping::toQuote(pByteData, pDoubleData));
      }
      case DXFG_EVENT_PROFILE: {
        return r_cast<dxfg_event_type_t*>(ProfileMapping::toProfile(pByteData, pDoubleData));
      }
      case DXFG_EVENT_SUMMARY: {
        return r_cast<dxfg_event_type_t*>(SummaryMapping::toSummary(pByteData, pDoubleData));
      }
      case DXFG_EVENT_GREEKS: {
        return r_cast<dxfg_event_type_t*>(GreeksMapping::toGreeks(pByteData, pDoubleData));
      }
      case DXFG_EVENT_CANDLE: {
        return r_cast<dxfg_event_type_t*>(CandleMapping::toCandle(pByteData, pDoubleData));
      }
      case DXFG_EVENT_UNDERLYING: {
        return r_cast<dxfg_event_type_t*>(UnderlyingMapping::toUnderlying(pByteData, pDoubleData));
      }
      case DXFG_EVENT_THEO_PRICE: {
        return r_cast<dxfg_event_type_t*>(TheoPriceMapping::toTheoPrice(pByteData, pDoubleData));
      }
      case DXFG_EVENT_TRADE_ETH:
      case DXFG_EVENT_TRADE: {
        return r_cast<dxfg_event_type_t*>(TradeMapping::toTradeBase(pByteData, pDoubleData));
      }
      case DXFG_EVENT_CONFIGURATION: {
        return r_cast<dxfg_event_type_t*>(toConfiguration(pByteData));
      }
      case DXFG_EVENT_MESSAGE: {
        return r_cast<dxfg_event_type_t*>(toMessage(pByteData));
      }
      case DXFG_EVENT_TIME_AND_SALE: {
        return r_cast<dxfg_event_type_t*>(TimeAndSaleMapping::toTimeAndSale(pByteData, pDoubleData));
      }
      case DXFG_EVENT_ORDER_BASE: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toOrderBase(pByteData, pDoubleData));
      }
      case DXFG_EVENT_ORDER: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toOrder(pByteData, pDoubleData));
      }
      case DXFG_EVENT_ANALYTIC_ORDER: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toAnalyticsOrder(pByteData, pDoubleData));
      }
      case DXFG_EVENT_SPREAD_ORDER: {
        return r_cast<dxfg_event_type_t*>(OrderMapping::toSpreadOrder(pByteData, pDoubleData));
      }
      case DXFG_EVENT_SERIES: {
        return r_cast<dxfg_event_type_t*>(SeriesMapping::toSeries(pByteData, pDoubleData));
      }
      case DXFG_EVENT_OPTION_SALE: {
        return r_cast<dxfg_event_type_t*>(OptionSaleMapping::toOptionSale(pByteData, pDoubleData));
      }
      default: {
        javaLogger->info("NativeEventReader::toEvent = ", nullptr);
        return nullptr;
      }
    }
  }

  dxfg_configuration_t* NativeEventReader::toConfiguration(const char** pByteData) {
    auto* configuration = new dxfg_configuration_t();// todo: make wrapper to get attachment fromJava
    configuration->event_type.clazz = DXFG_EVENT_CONFIGURATION;
    configuration->event_symbol = EventReader::readString(pByteData);
    configuration->event_time = EventReader::readLong(pByteData);
    configuration->version = EventReader::readInt(pByteData);
    // todo: read next ID of type LONG, ID points to java.lang.Object attachment in Java
    // configuration->attachment = readBlob();
    return configuration;
  }

  dxfg_message_t* NativeEventReader::toMessage(const char** pByteData) {
    auto* message = new dxfg_message_t(); // todo: make wrapper to get attachment from Java
    message->event_type.clazz = DXFG_EVENT_MESSAGE;
    message->event_symbol = EventReader::readString(pByteData);
    message->event_time = EventReader::readLong(pByteData);
    // todo: read next ID of type LONG, ID points to java.lang.Object attachment in Java
    // message->attachment = readBlob(pByteData);
    return message;
  }
}