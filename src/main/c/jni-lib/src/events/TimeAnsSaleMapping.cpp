// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/TimeAndSaleMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_time_and_sale_t* TimeAndSaleMapping::toTimeAndSale(const char** pByteData, const double** pDoubleData) {
    auto* tns = new dxfg_time_and_sale_t();
    tns->market_event.event_type.clazz = DXFG_EVENT_TIME_AND_SALE;
    tns->market_event.event_symbol = EventReader::readString(pByteData);
    tns->market_event.event_time = EventReader::readLong(pByteData);
    tns->event_flags = EventReader::readInt(pByteData);
    tns->index = EventReader::readLong(pByteData);
    tns->time_nano_part = EventReader::readInt(pByteData);
    tns->exchange_code = EventReader::readInt16_t(pByteData);
    tns->flags = EventReader::readInt(pByteData);
    tns->exchange_sale_conditions = EventReader::readString(pByteData);
    tns->buyer = EventReader::readString(pByteData);
    tns->seller = EventReader::readString(pByteData);

    tns->price = EventReader::readDouble(pDoubleData);
    tns->size = EventReader::readDouble(pDoubleData);
    tns->bid_price = EventReader::readDouble(pDoubleData);
    tns->ask_price = EventReader::readDouble(pDoubleData);
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