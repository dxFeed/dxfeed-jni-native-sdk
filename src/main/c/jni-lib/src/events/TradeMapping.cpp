// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TradeMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_trade_base_t* TradeMapping::toTradeBase(const char** pByteData, const double** pDoubleData) {
    auto* trade = new dxfg_trade_base_t();
    trade->market_event.event_type.clazz = DXFG_EVENT_TRADE;
    trade->market_event.event_symbol = ByteReader::readString(pByteData);
    trade->market_event.event_time = ByteReader::readLong(pByteData);
    trade->time_sequence = ByteReader::readLong(pByteData);
    trade->time_nano_part = ByteReader::readInt(pByteData);
    trade->exchange_code = ByteReader::readInt16_t(pByteData);
    trade->day_id = ByteReader::readInt(pByteData);
    trade->flags = ByteReader::readInt(pByteData);

    trade->price = ByteReader::readDouble(pDoubleData);
    trade->change = ByteReader::readDouble(pDoubleData);
    trade->size = ByteReader::readDouble(pDoubleData);
    trade->day_volume = ByteReader::readDouble(pDoubleData);
    trade->day_turnover = ByteReader::readDouble(pDoubleData);
    return trade;
  }

  void TradeMapping::fromTradeBase(dxfg_trade_base_t* trade, ByteWriter& writer) {
    writer.writeString(trade->market_event.event_symbol);
    writer.writeInt64_t(trade->market_event.event_time);
    writer.writeInt64_t(trade->time_sequence);
    writer.writeInt32_t(trade->time_nano_part);
    writer.writeInt16_t(trade->exchange_code);
    writer.writeInt32_t(trade->day_id);
    writer.writeInt32_t(trade->flags);

    writer.writeDouble(trade->price);
    writer.writeDouble(trade->change);
    writer.writeDouble(trade->size);
    writer.writeDouble(trade->day_volume);
    writer.writeDouble(trade->day_turnover);
  }
}