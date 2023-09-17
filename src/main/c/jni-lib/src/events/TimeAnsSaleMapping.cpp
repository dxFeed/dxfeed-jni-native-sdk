// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TimeAndSaleMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {

  dxfg_time_and_sale_t* TimeAndSaleMapping::toTimeAndSale(const char** pByteData, const double** pDoubleData) {
    auto* tns = new dxfg_time_and_sale_t();
    tns->market_event.event_type.clazz = DXFG_EVENT_TIME_AND_SALE;
    tns->market_event.event_symbol = ByteReader::readString(pByteData);
    tns->market_event.event_time = ByteReader::readLong(pByteData);
    tns->event_flags = ByteReader::readInt(pByteData);
    tns->index = ByteReader::readLong(pByteData);
    tns->time_nano_part = ByteReader::readInt(pByteData);
    tns->exchange_code = ByteReader::readInt16_t(pByteData);
    tns->flags = ByteReader::readInt(pByteData);
    tns->exchange_sale_conditions = ByteReader::readString(pByteData);
    tns->buyer = ByteReader::readString(pByteData);
    tns->seller = ByteReader::readString(pByteData);

    tns->price = ByteReader::readDouble(pDoubleData);
    tns->size = ByteReader::readDouble(pDoubleData);
    tns->bid_price = ByteReader::readDouble(pDoubleData);
    tns->ask_price = ByteReader::readDouble(pDoubleData);
    return tns;
  }

  void TimeAndSaleMapping::fromTimeAndSale(dxfg_time_and_sale_t* tns, ByteWriter& writer) {
    writer.writeString(tns->market_event.event_symbol);
    writer.writeInt64_t(tns->market_event.event_time);
    writer.writeInt32_t(tns->event_flags);
    writer.writeInt64_t(tns->index);
    writer.writeInt64_t(tns->time_nano_part);
    writer.writeInt16_t(tns->exchange_code);
    writer.writeInt32_t(tns->flags);
    writer.writeString(tns->exchange_sale_conditions);
    writer.writeString(tns->buyer);
    writer.writeString(tns->seller);

    writer.writeDouble(tns->price);
    writer.writeDouble(tns->size);
    writer.writeDouble(tns->bid_price);
    writer.writeDouble(tns->ask_price);
  }
}