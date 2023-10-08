// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/UnderlyingMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_underlying_t* UnderlyingMapping::toUnderlying(ByteReader& reader) {
    auto* underlying = new dxfg_underlying_t();
    underlying->market_event.event_type.clazz = DXFG_EVENT_UNDERLYING;
    underlying->market_event.event_symbol = reader.readString();
    underlying->market_event.event_time = reader.readLong();
    underlying->event_flags = reader.readInt();
    underlying->index = reader.readLong();

    underlying->volatility = reader.readDouble();
    underlying->front_volatility = reader.readDouble();
    underlying->back_volatility = reader.readDouble();
    underlying->call_volume = reader.readDouble();
    underlying->put_volume = reader.readDouble();
    underlying->put_call_ratio = reader.readDouble();
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