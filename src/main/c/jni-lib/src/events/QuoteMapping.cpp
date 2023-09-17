// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/QuoteMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_quote_t* QuoteMapping::toQuote(const char** pByteData, const double** pDoubleData) {
    auto* quote = new dxfg_quote_t();
    quote->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
    quote->market_event.event_symbol = ByteReader::readString(pByteData);
    quote->market_event.event_time = ByteReader::readLong(pByteData);
    quote->time_millis_sequence = ByteReader::readInt(pByteData);
    quote->time_nano_part = ByteReader::readInt(pByteData);
    quote->bid_time = ByteReader::readLong(pByteData);
    quote->bid_exchange_code = ByteReader::readInt16_t(pByteData);
    quote->ask_time = ByteReader::readLong(pByteData);
    quote->ask_exchange_code = ByteReader::readInt16_t(pByteData);

    quote->bid_price = ByteReader::readDouble(pDoubleData);
    quote->bid_size = ByteReader::readDouble(pDoubleData);
    quote->ask_price = ByteReader::readDouble(pDoubleData);
    quote->ask_size = ByteReader::readDouble(pDoubleData);
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