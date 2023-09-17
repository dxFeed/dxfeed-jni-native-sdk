// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TheoPriceMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_theo_price_t* TheoPriceMapping::toTheoPrice(const char** pByteData, const double** pDoubleData) {
    auto* theoPrice = new dxfg_theo_price_t();
    theoPrice->market_event.event_type.clazz = DXFG_EVENT_THEO_PRICE;
    theoPrice->market_event.event_symbol = ByteReader::readString(pByteData);
    theoPrice->market_event.event_time = ByteReader::readLong(pByteData);
    theoPrice->event_flags = ByteReader::readInt(pByteData);
    theoPrice->index = ByteReader::readLong(pByteData);

    theoPrice->price = ByteReader::readDouble(pDoubleData);
    theoPrice->underlying_price = ByteReader::readDouble(pDoubleData);
    theoPrice->delta = ByteReader::readDouble(pDoubleData);
    theoPrice->gamma = ByteReader::readDouble(pDoubleData);
    theoPrice->dividend = ByteReader::readDouble(pDoubleData);
    theoPrice->interest = ByteReader::readDouble(pDoubleData);
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