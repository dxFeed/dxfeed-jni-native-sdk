// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/ByteWriter.hpp"
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
  ByteWriter::ByteWriter(int eventCount) {
    auto totalSize_ = eventCount * EVENT_SIZE_IN_BYTES;
    eventTypes_.reserve(totalSize_);
    byteData_.reserve(totalSize_);
    doubleData_.reserve(totalSize_);
  }

  ByteWriter::~ByteWriter() {
    eventTypes_.clear();
    byteData_.clear();
    doubleData_.clear();
  }

  void ByteWriter::writeBytes(const int8_t* bytes, int32_t len) {
    if (byteData_.size() + len >= byteData_.capacity()) {
      byteData_.resize(byteData_.capacity() * 2);
    }
    // http://thewayofc.blogspot.com/2014/05/copying-memory-from-c-to-c-using.html
    byteData_.insert(byteData_.end(), bytes, bytes + len);
  }

  void ByteWriter::writeInt16_t(int16_t value) {
    byteData_.push_back((value >> 8) & 0xFF);
    byteData_.push_back(value        & 0xFF);
  }

  void ByteWriter::writeInt32_t(int32_t value) {
    byteData_.push_back((value >> 24) & 0xFF);
    byteData_.push_back((value >> 16) & 0xFF);
    byteData_.push_back((value >> 8) & 0xFF);
    byteData_.push_back(value        & 0xFF);
  }

  void ByteWriter::writeInt64_t(int64_t value) {
    byteData_.push_back((value >> 56) & 0xFF);
    byteData_.push_back((value >> 48) & 0xFF);
    byteData_.push_back((value >> 40) & 0xFF);
    byteData_.push_back((value >> 32) & 0xFF);
    byteData_.push_back((value >> 24) & 0xFF);
    byteData_.push_back((value >> 16) & 0xFF);
    byteData_.push_back((value >> 8) & 0xFF);
    byteData_.push_back(value        & 0xFF);
  }

  void ByteWriter::writeDouble(double value) {
    doubleData_.push_back(value);
  }

  void ByteWriter::writeString(const char* str) {
    auto strSize = static_cast<int16_t>(str ? strlen(str) : 0);
    writeInt16_t(strSize);
    if (strSize) {
      static_assert(sizeof(int8_t) == sizeof(char));
      writeBytes(r_cast<const int8_t*>(str), strSize);
    }
  }

  void ByteWriter::writeEvent(dxfg_event_type_t* eventType) {
    dxfg_event_clazz_t dxfgEventClazz = eventType->clazz;
    eventTypes_.push_back(dxfgEventClazz);
    switch (dxfgEventClazz) {
      case DXFG_EVENT_QUOTE: {
        QuoteMapping::fromQuote(r_cast<dxfg_quote_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_PROFILE: {
        ProfileMapping::fromProfile(r_cast<dxfg_profile_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_SUMMARY: {
        SummaryMapping::fromSummary(r_cast<dxfg_summary_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_GREEKS: {
        GreeksMapping::fromGreeks(r_cast<dxfg_greeks_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_CANDLE: {
        CandleMapping::fromCandle(r_cast<dxfg_candle_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_UNDERLYING: {
        UnderlyingMapping::fromUnderlying(r_cast<dxfg_underlying_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_THEO_PRICE: {
        TheoPriceMapping::fromTheoPrice(r_cast<dxfg_theo_price_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_TRADE:
      case DXFG_EVENT_TRADE_ETH: {
        TradeMapping::fromTradeBase(r_cast<dxfg_trade_base_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_CONFIGURATION: {
        ConfigurationMapping::fromConfiguration(r_cast<dxfg_configuration_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_MESSAGE: {
        MessageMapping::fromMessage(r_cast<dxfg_message_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_TIME_AND_SALE: {
        TimeAndSaleMapping::fromTimeAndSale(r_cast<dxfg_time_and_sale_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_ORDER_BASE: {
        OrderMapping::fromOrderBase(r_cast<dxfg_order_base_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_ORDER: {
        OrderMapping::fromOrder(r_cast<dxfg_order_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_ANALYTIC_ORDER: {
        OrderMapping::fromAnalyticOrder(r_cast<dxfg_analytic_order_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_SPREAD_ORDER: {
        OrderMapping::fromSpreadOrder(r_cast<dxfg_spread_order_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_SERIES: {
        SeriesMapping::fromSeries(r_cast<dxfg_series_t*>(eventType), *this);
        return;
      }
      case DXFG_EVENT_OPTION_SALE: {
        OptionSaleMapping::fromOptionSale(r_cast<dxfg_option_sale_t*>(eventType), *this);
        return;
      }
    }
  }

  jbyteArray ByteWriter::eventTypes(JNIEnv* env) {
    auto size = static_cast<int32_t>(eventTypes_.size());
    auto pArray = env->NewByteArray(size);
    auto data = r_cast<char*>(env->GetPrimitiveArrayCritical(pArray, 0));
    memcpy(data, eventTypes_.data(), size);
    env->ReleasePrimitiveArrayCritical(pArray, data, 0);
    return pArray;
  }

  jbyteArray ByteWriter::byteData(JNIEnv* env) {
    auto size = static_cast<int32_t>(byteData_.size());
    auto pArray = env->NewByteArray(size);
    auto data = r_cast<char*>(env->GetPrimitiveArrayCritical(pArray, 0));
    memcpy(data, byteData_.data(), size);
    env->ReleasePrimitiveArrayCritical(pArray, data, 0);
    return pArray;
  }

  jdoubleArray ByteWriter::doubleData(JNIEnv* env) {
    auto size = static_cast<int32_t>(doubleData_.size());
    auto pArray = env->NewDoubleArray(size);
    auto data = r_cast<double*>(env->GetPrimitiveArrayCritical(pArray, 0));
    memcpy(data, doubleData_.data(), size * sizeof(double));
    env->ReleasePrimitiveArrayCritical(pArray, data, 0);
    return pArray;
  }
}