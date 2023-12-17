// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api.serializers;

import com.dxfeed.api.DxfgEventClazzT;
import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Trade;
import com.dxfeed.event.market.TradeBase;
import com.dxfeed.event.market.TradeETH;

public class TradeMapping {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L286
   *
   * typedef struct dxfg_trade_base_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_lasting_event_t lasting_event;
   *    int64_t time_sequence;
   *    int32_t time_nano_part;
   *    int16_t exchange_code;
   *    double price;
   *    double change;
   *    double size;
   *    int32_t day_id;
   *    double day_volume;
   *    double day_turnover;
   *    int32_t flags;
   * } dxfg_trade_base_t;
   */
  public static <T> byte toNative(T eventT, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    TradeBase event = (TradeBase) eventT;
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                       // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                           // 8
    pBytes.writeLong(event.getTimeSequence());                        // 8
    pBytes.writeInt(event.getTimeNanoPart());                         // 4
    pBytes.writeChar(event.getExchangeCode());                        // 2
    pBytes.writeInt(event.getDayId());                                // 4
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getFlags(event)); // 4

    // DOUBLE DATA
    pDoubles.write(event.getPrice());
    pDoubles.write(event.getChange());
    pDoubles.write(event.getSizeAsDouble());
    pDoubles.write(event.getDayVolumeAsDouble());
    pDoubles.write(event.getDayTurnover());

    boolean isETH = eventT instanceof TradeETH;
    return isETH ? DxfgEventClazzT.DXFG_EVENT_TRADE_ETH : DxfgEventClazzT.DXFG_EVENT_TRADE;
  }

  public static Trade fromNative(ByteReader reader) {
    Trade trade = new Trade();
    trade.setEventSymbol(reader.readString());
    trade.setEventTime(reader.readLong());
    trade.setTimeSequence(reader.readLong());
    trade.setTimeNanoPart(reader.readInt());
    trade.setExchangeCode(reader.readChar());
    trade.setDayId(reader.readInt());
    DxFeedEventMarketPackagePrivate.setFlags(trade, reader.readInt());

    trade.setPrice(reader.readDouble());
    trade.setChange(reader.readDouble());
    trade.setSizeAsDouble(reader.readDouble());
    trade.setDayVolumeAsDouble(reader.readDouble());
    trade.setDayTurnover(reader.readDouble());
    return trade;
  }
}
