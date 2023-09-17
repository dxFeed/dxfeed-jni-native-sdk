// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/UnderlyingMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_underlying_t* UnderlyingMapping::toUnderlying(const char** pByteData, const double** pDoubleData) {
    auto* underlying = new dxfg_underlying_t();
    underlying->market_event.event_type.clazz = DXFG_EVENT_UNDERLYING;
    underlying->market_event.event_symbol = ByteReader::readString(pByteData);
    underlying->market_event.event_time = ByteReader::readLong(pByteData);
    underlying->event_flags = ByteReader::readInt(pByteData);
    underlying->index = ByteReader::readLong(pByteData);

    underlying->volatility = ByteReader::readDouble(pDoubleData);
    underlying->front_volatility = ByteReader::readDouble(pDoubleData);
    underlying->back_volatility = ByteReader::readDouble(pDoubleData);
    underlying->call_volume = ByteReader::readDouble(pDoubleData);
    underlying->put_volume = ByteReader::readDouble(pDoubleData);
    underlying->put_call_ratio = ByteReader::readDouble(pDoubleData);
    return underlying;
  }

  void UnderlyingMapping::fromUnderlying(dxfg_underlying_t* underlying, ByteWriter& writer) {
    writer.writeString(underlying->market_event.event_symbol);
    writer.writeInt64_t(underlying->market_event.event_time);
    writer.writeInt32_t(underlying->event_flags);
    writer.writeInt64_t(underlying->index);

    writer.writeDouble(underlying->volatility);
    writer.writeDouble(underlying->front_volatility);
    writer.writeDouble(underlying->back_volatility);
    writer.writeDouble(underlying->call_volume);
    writer.writeDouble(underlying->put_volume);
    writer.writeDouble(underlying->put_call_ratio);
  }
}