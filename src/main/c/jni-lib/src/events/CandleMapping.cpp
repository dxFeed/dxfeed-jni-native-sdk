// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/CandleMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_candle_t* CandleMapping::toCandle(const char** pByteData, const double** pDoubleData) {
    auto* candle = new dxfg_candle_t();
    candle->event_type.clazz = DXFG_EVENT_CANDLE;
    candle->event_symbol = ByteReader::readString(pByteData);
    candle->event_time = ByteReader::readLong(pByteData);
    candle->event_flags = ByteReader::readInt(pByteData);
    candle->index = ByteReader::readLong(pByteData);
    candle->count = ByteReader::readLong(pByteData);

    candle->open = ByteReader::readDouble(pDoubleData);
    candle->high = ByteReader::readDouble(pDoubleData);
    candle->low = ByteReader::readDouble(pDoubleData);
    candle->close = ByteReader::readDouble(pDoubleData);
    candle->volume = ByteReader::readDouble(pDoubleData);
    candle->vwap = ByteReader::readDouble(pDoubleData);
    candle->bid_volume = ByteReader::readDouble(pDoubleData);
    candle->ask_volume = ByteReader::readDouble(pDoubleData);
    candle->imp_volatility = ByteReader::readDouble(pDoubleData);
    candle->open_interest = ByteReader::readDouble(pDoubleData);
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