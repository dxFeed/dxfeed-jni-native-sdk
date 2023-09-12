package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Trade;
import com.dxfeed.event.market.TradeBase;

public class TradeToNative {
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
  public static void convert(TradeBase event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
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
    pDoubles.write(event.getSize());
    pDoubles.write(event.getDayVolume());
    pDoubles.write(event.getDayTurnover());
  }

  public static Trade fromNative(NativeEventsReader eventsReader) {
    Trade trade = new Trade();

    trade.setEventSymbol(eventsReader.readString());
    trade.setEventTime(eventsReader.readLong());
    trade.setTimeSequence(eventsReader.readLong());
    trade.setTimeNanoPart(eventsReader.readInt());
    trade.setExchangeCode(eventsReader.readChar());
    trade.setDayId(eventsReader.readInt());
    DxFeedEventMarketPackagePrivate.setFlags(trade, eventsReader.readInt());

    trade.setPrice(eventsReader.readDouble());
    trade.setChange(eventsReader.readDouble());
    trade.setSizeAsDouble(eventsReader.readDouble());
    trade.setDayVolumeAsDouble(eventsReader.readDouble());
    trade.setDayTurnover(eventsReader.readDouble());

    return trade;
  }
}
