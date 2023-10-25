// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TradeMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_trade_base_t* TradeMapping::toTradeBase(ByteReader& reader, dxfg_event_clazz_t dxfgEventClazz)
  {
    auto* trade = new dxfg_trade_base_t();
    trade->market_event.event_type.clazz = dxfgEventClazz;
    trade->market_event.event_symbol = reader.readString();
    trade->market_event.event_time = reader.readLong();
    trade->time_sequence = reader.readLong();
    trade->time_nano_part = reader.readInt();
    trade->exchange_code = reader.readInt16_t();
    trade->day_id = reader.readInt();
    trade->flags = reader.readInt();

    trade->price = reader.readDouble();
    trade->change = reader.readDouble();
    trade->size = reader.readDouble();
    trade->day_volume = reader.readDouble();
    trade->day_turnover = reader.readDouble();
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