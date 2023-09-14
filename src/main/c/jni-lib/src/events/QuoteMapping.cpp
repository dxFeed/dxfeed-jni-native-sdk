// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/QuoteMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_quote_t* QuoteMapping::toQuote(const char** pByteData, const double** pDoubleData) {
    auto* quote = new dxfg_quote_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
    quote->market_event.event_symbol = EventReader::readString(pByteData);
    quote->market_event.event_time = EventReader::readLong(pByteData);
    quote->time_millis_sequence = EventReader::readInt(pByteData);
    quote->time_nano_part = EventReader::readInt(pByteData);
    quote->bid_time = EventReader::readLong(pByteData);
    quote->bid_exchange_code = EventReader::readInt16_t(pByteData);
    quote->ask_time = EventReader::readLong(pByteData);
    quote->ask_exchange_code = EventReader::readInt16_t(pByteData);

    quote->bid_price = EventReader::readDouble(pDoubleData);
    quote->bid_size = EventReader::readDouble(pDoubleData);
    quote->ask_price = EventReader::readDouble(pDoubleData);
    quote->ask_size = EventReader::readDouble(pDoubleData);
    return quote;
  }

  void QuoteMapping::fromQuote(dxfg_quote_t* quote, NativeEventWriter& writer) {
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