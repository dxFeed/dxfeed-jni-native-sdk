// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/CandleMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_candle_t* CandleMapping::toCandle(const char** pByteData, const double** pDoubleData) {
    auto* candle = new dxfg_candle_t();
    candle->event_type.clazz = DXFG_EVENT_CANDLE;
    candle->event_symbol = EventReader::readString(pByteData);
    candle->event_time = EventReader::readLong(pByteData);
    candle->event_flags = EventReader::readInt(pByteData);
    candle->index = EventReader::readLong(pByteData);
    candle->count = EventReader::readLong(pByteData);

    candle->open = EventReader::readDouble(pDoubleData);
    candle->high = EventReader::readDouble(pDoubleData);
    candle->low = EventReader::readDouble(pDoubleData);
    candle->close = EventReader::readDouble(pDoubleData);
    candle->volume = EventReader::readDouble(pDoubleData);
    candle->vwap = EventReader::readDouble(pDoubleData);
    candle->bid_volume = EventReader::readDouble(pDoubleData);
    candle->ask_volume = EventReader::readDouble(pDoubleData);
    candle->imp_volatility = EventReader::readDouble(pDoubleData);
    candle->open_interest = EventReader::readDouble(pDoubleData);
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