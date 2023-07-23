package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.TradeBase;

public class TradeToNative {
  /**
   * typedef struct dxfg_market_event_t {
   * dxfg_event_type_t event_type;
   * const char *event_symbol;
   * int64_t event_time;
   * } dxfg_market_event_t;
   * <p>
   * // https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L286
   * typedef struct dxfg_trade_base_t {
   * dxfg_market_event_t market_event;
   * //    dxfg_lasting_event_t lasting_event;
   * int64_t time_sequence;
   * int32_t time_nano_part;
   * int16_t exchange_code;
   * double price;
   * double change;
   * double size;
   * int32_t day_id;
   * double day_volume;
   * double day_turnover;
   * int32_t flags;
   * } dxfg_trade_base_t;
   */
  public static void convert(TradeBase event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                  // 8
    long timeSequence = event.getTimeSequence();                            // 8
    int timeNanoPart = event.getTimeNanoPart();                             // 4
    char exchangeCode = event.getExchangeCode();                            // 2
    int dayId = event.getDayId();                                           // 4
    int flags = DxFeedEventMarketPackagePrivate.getFlags(event);            // 4

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (8) + (4) + (2) + (4) + (4);

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);        // 8
    pBytes.writeLong(timeSequence);     // 8
    pBytes.writeInt(timeNanoPart);      // 4
    pBytes.writeChar(exchangeCode);     // 2
    pBytes.writeInt(dayId);             // 4
    pBytes.writeInt(flags);             // 4

    // DOUBLE DATA
    double price = event.getPrice();              // 1
    double change = event.getChange();            // 1
    double size = event.getSize();                // 1
    double dayVolume = event.getDayVolume();      // 1
    double dayTurnover = event.getDayTurnover();  // 1

    // DOUBLE DATA
    pDoubles.addChunk(chunkIdx, 5);
    pDoubles.write(price);
    pDoubles.write(change);
    pDoubles.write(size);
    pDoubles.write(dayVolume);
    pDoubles.write(dayTurnover);
  }
}
