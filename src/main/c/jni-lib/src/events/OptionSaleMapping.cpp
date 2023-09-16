// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/OptionSaleMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_option_sale_t* OptionSaleMapping::toOptionSale(const char** pByteData, const double** pDoubleData) {
    auto* optionSale = new dxfg_option_sale_t();
    optionSale->market_event.event_type.clazz = DXFG_EVENT_OPTION_SALE;
    optionSale->market_event.event_symbol = EventReader::readString(pByteData);
    optionSale->market_event.event_time = EventReader::readLong(pByteData);
    optionSale->event_flags = EventReader::readInt(pByteData);
    optionSale->index = EventReader::readLong(pByteData);
    optionSale->time_sequence = EventReader::readLong(pByteData);
    optionSale->time_nano_part = EventReader::readInt(pByteData);
    optionSale->exchange_code = EventReader::readInt16_t(pByteData);
    optionSale->flags = EventReader::readInt(pByteData);
    optionSale->exchange_sale_conditions = EventReader::readString(pByteData);
    optionSale->option_symbol = EventReader::readString(pByteData);

    optionSale->price = EventReader::readDouble(pDoubleData);
    optionSale->size = EventReader::readDouble(pDoubleData);
    optionSale->bid_price = EventReader::readDouble(pDoubleData);
    optionSale->ask_price = EventReader::readDouble(pDoubleData);
    optionSale->underlying_price = EventReader::readDouble(pDoubleData);
    optionSale->volatility = EventReader::readDouble(pDoubleData);
    optionSale->delta = EventReader::readDouble(pDoubleData);
    return optionSale;
  }

  void OptionSaleMapping::fromOptionSale(dxfg_option_sale_t* optionSale, ByteWriter& writer) {
    writer.writeString(optionSale->market_event.event_symbol);
    writer.writeInt64_t(optionSale->market_event.event_time);
    writer.writeInt32_t(optionSale->event_flags);
    writer.writeInt64_t(optionSale->index);
    writer.writeInt64_t(optionSale->time_sequence);
    writer.writeInt32_t(optionSale->time_nano_part);
    writer.writeInt16_t(optionSale->exchange_code);
    writer.writeInt32_t(optionSale->flags);
    writer.writeString(optionSale->exchange_sale_conditions);
    writer.writeString(optionSale->option_symbol);

    writer.writeDouble(optionSale->price);
    writer.writeDouble(optionSale->size);
    writer.writeDouble(optionSale->bid_price);
    writer.writeDouble(optionSale->ask_price);
    writer.writeDouble(optionSale->underlying_price);
    writer.writeDouble(optionSale->volatility);
    writer.writeDouble(optionSale->delta);
  }
}