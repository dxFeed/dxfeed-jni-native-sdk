package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.candle.CandleSymbol;

public class CandleMapping {
  /**
   * typedef struct dxfg_candle_symbol_t {
   *    dxfg_symbol_t supper;
   *    const char* symbol;
   * } dxfg_candle_symbol_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L219
   *
   * typedef struct dxfg_candle_t {
   *    dxfg_event_type_t event_type;
   *    //  dxfg_time_series_event_t time_series_event;
   *    //  dxfg_lasting_event_t lasting_event;
   *    dxfg_candle_symbol_t *event_symbol;
   *    int32_t event_flags;
   *    int64_t event_time;
   *    int64_t index;
   *    int64_t count;
   *    double open;
   *    double high;
   *    double low;
   *    double close;
   *    double volume;
   *    double vwap;
   *    double bid_volume;
   *    double ask_volume;
   *    double imp_volatility;
   *    double open_interest;
   * } dxfg_candle_t;
   */

  // todo: sync about CandleSymbol
  public static void toNative(Candle event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol().toString());  // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                 // 8
    pBytes.writeInt(event.getEventFlags());                 // 4
    pBytes.writeLong(event.getIndex());                     // 8
    pBytes.writeLong(event.getCount());                     // 8

    // DOUBLE DATA
    pDoubles.write(event.getOpen());
    pDoubles.write(event.getHigh());
    pDoubles.write(event.getLow());
    pDoubles.write(event.getClose());
    pDoubles.write(event.getVolumeAsDouble());
    pDoubles.write(event.getVWAP());
    pDoubles.write(event.getBidVolumeAsDouble());
    pDoubles.write(event.getAskVolumeAsDouble());
    pDoubles.write(event.getImpVolatility());
    pDoubles.write(event.getOpenInterestAsDouble());
  }

  public static Candle fromNative(NativeEventsReader reader) {
    Candle candle = new Candle();
    candle.setEventSymbol(CandleSymbol.valueOf(reader.readString()));
    candle.setEventTime(reader.readLong());
    candle.setEventFlags(reader.readInt());
    candle.setIndex(reader.readLong());
    candle.setCount(reader.readLong());

    candle.setOpen(reader.readDouble());
    candle.setHigh(reader.readDouble());
    candle.setLow(reader.readDouble());
    candle.setClose(reader.readDouble());
    candle.setVolumeAsDouble(reader.readDouble());
    candle.setVWAP(reader.readDouble());
    candle.setBidVolumeAsDouble(reader.readDouble());
    candle.setAskVolumeAsDouble(reader.readDouble());
    candle.setImpVolatility(reader.readDouble());
    candle.setOpenInterestAsDouble(reader.readDouble());
    return candle;
  }
}
