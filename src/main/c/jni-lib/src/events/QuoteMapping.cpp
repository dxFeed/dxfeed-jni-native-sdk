// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/events/QuoteMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_quote_t* QuoteMapping::toQuote(ByteReader& reader) {
    auto* quote = new dxfg_quote_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
    quote->market_event.event_symbol = reader.readString();
    quote->market_event.event_time = reader.readLong();
    quote->time_millis_sequence = reader.readInt();
    quote->time_nano_part = reader.readInt();
    quote->bid_time = reader.readLong();
    quote->bid_exchange_code = reader.readInt16_t();
    quote->ask_time = reader.readLong();
    quote->ask_exchange_code = reader.readInt16_t();

    quote->bid_price = reader.readDouble();
    quote->bid_size = reader.readDouble();
    quote->ask_price = reader.readDouble();
    quote->ask_size = reader.readDouble();
    return quote;
  }

  void QuoteMapping::fromQuote(dxfg_quote_t* quote, ByteWriter& writer) {
    writer.writeString(quote->market_event.event_symbol);
    writer.writeInt64_t(quote->market_event.event_time);
    writer.writeInt32_t(quote->time_millis_sequence);
    writer.writeInt32_t(quote->time_nano_part);
    writer.writeInt64_t(quote->bid_time);
    writer.writeInt16_t(quote->bid_exchange_code);
    writer.writeInt64_t(quote->ask_time);
    writer.writeInt16_t(quote->ask_exchange_code);

    writer.writeDouble(quote->bid_price);
    writer.writeDouble(quote->bid_size);
    writer.writeDouble(quote->ask_price);
    writer.writeDouble(quote->ask_size);
  }
}