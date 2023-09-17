// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/GreeksMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_greeks_t* GreeksMapping::toGreeks(const char** pByteData, const double** pDoubleData) {
    auto greeks = new dxfg_greeks_t();
    greeks->market_event.event_type.clazz = DXFG_EVENT_GREEKS;
    greeks->market_event.event_symbol = ByteReader::readString(pByteData);
    greeks->market_event.event_time = ByteReader::readLong(pByteData);
    greeks->event_flags = ByteReader::readInt(pByteData);
    greeks->index = ByteReader::readLong(pByteData);

    greeks->price = ByteReader::readDouble(pDoubleData);
    greeks->volatility = ByteReader::readDouble(pDoubleData);
    greeks->delta = ByteReader::readDouble(pDoubleData);
    greeks->gamma = ByteReader::readDouble(pDoubleData);
    greeks->theta = ByteReader::readDouble(pDoubleData);
    greeks->rho = ByteReader::readDouble(pDoubleData);
    greeks->vega = ByteReader::readDouble(pDoubleData);
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