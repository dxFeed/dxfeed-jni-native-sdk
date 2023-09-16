// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/UnderlyingMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_underlying_t* UnderlyingMapping::toUnderlying(const char** pByteData, const double** pDoubleData) {
    auto* underlying = new dxfg_underlying_t();
    underlying->market_event.event_type.clazz = DXFG_EVENT_UNDERLYING;
    underlying->market_event.event_symbol = EventReader::readString(pByteData);
    underlying->market_event.event_time = EventReader::readLong(pByteData);
    underlying->event_flags = EventReader::readInt(pByteData);
    underlying->index = EventReader::readLong(pByteData);

    underlying->volatility = EventReader::readDouble(pDoubleData);
    underlying->front_volatility = EventReader::readDouble(pDoubleData);
    underlying->back_volatility = EventReader::readDouble(pDoubleData);
    underlying->call_volume = EventReader::readDouble(pDoubleData);
    underlying->put_volume = EventReader::readDouble(pDoubleData);
    underlying->put_call_ratio = EventReader::readDouble(pDoubleData);

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