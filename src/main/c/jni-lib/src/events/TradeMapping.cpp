// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TradeMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_trade_base_t* TradeMapping::toTradeBase(const char** pByteData, const double** pDoubleData) {
    auto* trade = new dxfg_trade_base_t();
    trade->market_event.event_type.clazz = DXFG_EVENT_TRADE;
    trade->market_event.event_symbol = EventReader::readString(pByteData);
    trade->market_event.event_time = EventReader::readLong(pByteData);
    trade->time_sequence = EventReader::readLong(pByteData);
    trade->time_nano_part = EventReader::readInt(pByteData);
    trade->exchange_code = EventReader::readInt16_t(pByteData);
    trade->day_id = EventReader::readInt(pByteData);
    trade->flags = EventReader::readInt(pByteData);

    trade->price = EventReader::readDouble(pDoubleData);
    trade->change = EventReader::readDouble(pDoubleData);
    trade->size = EventReader::readDouble(pDoubleData);
    trade->day_volume = EventReader::readDouble(pDoubleData);
    trade->day_turnover = EventReader::readDouble(pDoubleData);

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