// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/GreeksMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_greeks_t* GreeksMapping::toGreeks(const char** pByteData, const double** pDoubleData) {
    auto greeks = new dxfg_greeks_t();
    greeks->market_event.event_type.clazz = DXFG_EVENT_GREEKS;
    greeks->market_event.event_symbol = EventReader::readString(pByteData);
    greeks->market_event.event_time = EventReader::readLong(pByteData);
    greeks->event_flags = EventReader::readInt(pByteData);
    greeks->index = EventReader::readLong(pByteData);

    greeks->price = EventReader::readDouble(pDoubleData);
    greeks->volatility = EventReader::readDouble(pDoubleData);
    greeks->delta = EventReader::readDouble(pDoubleData);
    greeks->gamma = EventReader::readDouble(pDoubleData);
    greeks->theta = EventReader::readDouble(pDoubleData);
    greeks->rho = EventReader::readDouble(pDoubleData);
    greeks->vega = EventReader::readDouble(pDoubleData);
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