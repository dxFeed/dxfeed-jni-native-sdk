// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/CandleMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_candle_t* CandleMapping::toCandle(ByteReader& reader) {
    auto* candle = new dxfg_candle_t();
    candle->event_type.clazz = DXFG_EVENT_CANDLE;
    candle->event_symbol = reader.readString();
    candle->event_time = reader.readLong();
    candle->event_flags = reader.readInt();
    candle->index = reader.readLong();
    candle->count = reader.readLong();

    candle->open = reader.readDouble();
    candle->high = reader.readDouble();
    candle->low = reader.readDouble();
    candle->close = reader.readDouble();
    candle->volume = reader.readDouble();
    candle->vwap = reader.readDouble();
    candle->bid_volume = reader.readDouble();
    candle->ask_volume = reader.readDouble();
    candle->imp_volatility = reader.readDouble();
    candle->open_interest = reader.readDouble();
    return candle;
  }

  void CandleMapping::fromCandle(dxfg_candle_t* candle, ByteWriter& writer) {
    writer.writeString(candle->event_symbol);
    writer.writeInt64_t(candle->event_time);
    writer.writeInt32_t(candle->event_flags);
    writer.writeInt64_t(candle->index);
    writer.writeInt64_t(candle->count);

    writer.writeDouble(candle->open);
    writer.writeDouble(candle->high);
    writer.writeDouble(candle->low);
    writer.writeDouble(candle->close);
    writer.writeDouble(candle->volume);
    writer.writeDouble(candle->vwap);
    writer.writeDouble(candle->bid_volume);
    writer.writeDouble(candle->ask_volume);
    writer.writeDouble(candle->imp_volatility);
    writer.writeDouble(candle->open_interest);
  }
}