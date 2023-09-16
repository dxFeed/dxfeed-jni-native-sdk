// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TheoPriceMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_theo_price_t* TheoPriceMapping::toTheoPrice(const char** pByteData, const double** pDoubleData) {
    auto* theoPrice = new dxfg_theo_price_t();
    theoPrice->market_event.event_type.clazz = DXFG_EVENT_THEO_PRICE;
    theoPrice->market_event.event_symbol = EventReader::readString(pByteData);
    theoPrice->market_event.event_time = EventReader::readLong(pByteData);
    theoPrice->event_flags = EventReader::readInt(pByteData);
    theoPrice->index = EventReader::readLong(pByteData);

    theoPrice->price = EventReader::readDouble(pDoubleData);
    theoPrice->underlying_price = EventReader::readDouble(pDoubleData);
    theoPrice->delta = EventReader::readDouble(pDoubleData);
    theoPrice->gamma = EventReader::readDouble(pDoubleData);
    theoPrice->dividend = EventReader::readDouble(pDoubleData);
    theoPrice->interest = EventReader::readDouble(pDoubleData);

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