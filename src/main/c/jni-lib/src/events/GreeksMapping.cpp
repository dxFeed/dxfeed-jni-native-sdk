// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/GreeksMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_greeks_t* GreeksMapping::toGreeks(ByteReader& reader) {
    auto greeks = new dxfg_greeks_t();
    greeks->market_event.event_type.clazz = DXFG_EVENT_GREEKS;
    greeks->market_event.event_symbol = reader.readString();
    greeks->market_event.event_time = reader.readLong();
    greeks->event_flags = reader.readInt();
    greeks->index = reader.readLong();

    greeks->price = reader.readDouble();
    greeks->volatility = reader.readDouble();
    greeks->delta = reader.readDouble();
    greeks->gamma = reader.readDouble();
    greeks->theta = reader.readDouble();
    greeks->rho = reader.readDouble();
    greeks->vega = reader.readDouble();
    return greeks;
  }

  void GreeksMapping::fromGreeks(dxfg_greeks_t* greeks, ByteWriter& writer) {
    writer.writeString(greeks->market_event.event_symbol);
    writer.writeInt64_t(greeks->market_event.event_time);
    writer.writeInt32_t(greeks->event_flags);
    writer.writeInt64_t(greeks->index);

    writer.writeDouble(greeks->price);
    writer.writeDouble(greeks->volatility);
    writer.writeDouble(greeks->delta);
    writer.writeDouble(greeks->gamma);
    writer.writeDouble(greeks->theta);
    writer.writeDouble(greeks->rho);
    writer.writeDouble(greeks->vega);
  }
}