// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Quote;

public class QuoteMapping {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L140
   *
   * typedef struct dxfg_quote_t {
   *    dxfg_market_event_t market_event;
   *    int32_t time_millis_sequence;
   *    int32_t time_nano_part;
   *    int64_t bid_time;
   *    int16_t bid_exchange_code;
   *    double bid_price;
   *    double bid_size;
   *    int64_t ask_time;
   *    int16_t ask_exchange_code;
   *    double ask_price;
   *    double ask_size;
   * } dxfg_quote_t;
   */
  public static void toNative(Quote event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                                     // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                                         // 8
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getTimeMillisSequence(event));  // 4
    pBytes.writeInt(event.getTimeNanoPart());                                       // 4
    pBytes.writeLong(event.getBidTime());                                           // 8
    pBytes.writeChar(event.getBidExchangeCode());                                   // 2
    pBytes.writeLong(event.getAskTime());                                           // 8
    pBytes.writeChar(event.getAskExchangeCode());                                   // 2

    // DOUBLE DATA
    pDoubles.write(event.getBidPrice());
    pDoubles.write(event.getBidSizeAsDouble());
    pDoubles.write(event.getAskPrice());
    pDoubles.write(event.getAskSizeAsDouble());
  }

  public static Quote fromNative(ByteReader reader) {
    Quote quote = new Quote();
    quote.setEventSymbol(reader.readString());
    quote.setEventTime(reader.readLong());
    DxFeedEventMarketPackagePrivate.setTimeMillisSequence(quote, reader.readInt());
    quote.setTimeNanoPart(reader.readInt());
    quote.setBidTime(reader.readLong());
    quote.setBidExchangeCode(reader.readChar());
    quote.setAskTime(reader.readLong());
    quote.setAskExchangeCode(reader.readChar());

    quote.setBidPrice(reader.readDouble());
    quote.setBidSizeAsDouble(reader.readDouble());
    quote.setAskPrice(reader.readDouble());
    quote.setAskSizeAsDouble(reader.readDouble());
    return quote;
  }
}
