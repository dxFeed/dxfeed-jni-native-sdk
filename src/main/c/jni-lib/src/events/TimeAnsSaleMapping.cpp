// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TimeAndSaleMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {

  dxfg_time_and_sale_t* TimeAndSaleMapping::toTimeAndSale(ByteReader& reader) {
    auto* tns = new dxfg_time_and_sale_t();
    tns->market_event.event_type.clazz = DXFG_EVENT_TIME_AND_SALE;
    tns->market_event.event_symbol = reader.readString();
    tns->market_event.event_time = reader.readLong();
    tns->event_flags = reader.readInt();
    tns->index = reader.readLong();
    tns->time_nano_part = reader.readInt();
    tns->exchange_code = reader.readInt16_t();
    tns->flags = reader.readInt();
    tns->exchange_sale_conditions = reader.readString();
    tns->buyer = reader.readString();
    tns->seller = reader.readString();

    tns->price = reader.readDouble();
    tns->size = reader.readDouble();
    tns->bid_price = reader.readDouble();
    tns->ask_price = reader.readDouble();
    return tns;
  }

  void TimeAndSaleMapping::fromTimeAndSale(dxfg_time_and_sale_t* tns, ByteWriter& writer) {
    writer.writeString(tns->market_event.event_symbol);
    writer.writeInt64_t(tns->market_event.event_time);
    writer.writeInt32_t(tns->event_flags);
    writer.writeInt64_t(tns->index);
    writer.writeInt32_t(tns->time_nano_part);
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