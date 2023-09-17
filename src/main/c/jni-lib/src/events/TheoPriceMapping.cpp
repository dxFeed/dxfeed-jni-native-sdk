// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TheoPriceMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_theo_price_t* TheoPriceMapping::toTheoPrice(ByteReader& reader) {
    auto* theoPrice = new dxfg_theo_price_t();
    theoPrice->market_event.event_type.clazz = DXFG_EVENT_UNDERLYING;
    theoPrice->market_event.event_symbol = reader.readString();
    theoPrice->market_event.event_time = reader.readLong();
    theoPrice->event_flags = reader.readInt();
    theoPrice->index = reader.readLong();

    theoPrice->price = reader.readDouble();
    theoPrice->underlying_price = reader.readDouble();
    theoPrice->delta = reader.readDouble();
    theoPrice->gamma = reader.readDouble();
    theoPrice->dividend = reader.readDouble();
    theoPrice->interest = reader.readDouble();
    return theoPrice;
  }

  void TheoPriceMapping::fromTheoPrice(dxfg_theo_price_t* theoPrice, ByteWriter& writer) {
    writer.writeString(theoPrice->market_event.event_symbol);
    writer.writeInt64_t(theoPrice->market_event.event_time);
    writer.writeInt32_t(theoPrice->event_flags);
    writer.writeInt64_t(theoPrice->index);

    writer.writeDouble(theoPrice->price);
    writer.writeDouble(theoPrice->underlying_price);
    writer.writeDouble(theoPrice->delta);
    writer.writeDouble(theoPrice->gamma);
    writer.writeDouble(theoPrice->dividend);
    writer.writeDouble(theoPrice->interest);
  }
}