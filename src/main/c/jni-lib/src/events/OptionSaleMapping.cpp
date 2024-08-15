// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/OptionSaleMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
dxfg_option_sale_t *OptionSaleMapping::toOptionSale(ByteReader &reader) {
    auto *optionSale = new dxfg_option_sale_t();
    optionSale->market_event.event_type.clazz = DXFG_EVENT_OPTION_SALE;
    optionSale->market_event.event_symbol = reader.readString();
    optionSale->market_event.event_time = reader.readLong();
    optionSale->event_flags = reader.readInt();
    optionSale->index = reader.readLong();
    optionSale->time_sequence = reader.readLong();
    optionSale->time_nano_part = reader.readInt();
    optionSale->exchange_code = reader.readInt16_t();
    optionSale->flags = reader.readInt();
    optionSale->exchange_sale_conditions = reader.readString();
    optionSale->option_symbol = reader.readString();

    optionSale->price = reader.readDouble();
    optionSale->size = reader.readDouble();
    optionSale->bid_price = reader.readDouble();
    optionSale->ask_price = reader.readDouble();
    optionSale->underlying_price = reader.readDouble();
    optionSale->volatility = reader.readDouble();
    optionSale->delta = reader.readDouble();
    return optionSale;
}

void OptionSaleMapping::fromOptionSale(dxfg_option_sale_t *optionSale, ByteWriter &writer) {
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
} // namespace dxfeed::jni