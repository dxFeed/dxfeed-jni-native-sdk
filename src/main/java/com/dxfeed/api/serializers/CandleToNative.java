package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.candle.CandleSymbol;

public class CandleToNative {
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
  public static void convert(Candle event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol().toString());  // 2 + eventSymbolLength
    pBytes.writeInt(event.getEventFlags());                 // 4
    pBytes.writeLong(event.getEventTime());                 // 8
    pBytes.writeLong(event.getIndex());                     // 8
    pBytes.writeLong(event.getCount());                     // 8

    // DOUBLE DATA
    pDoubles.write(event.getOpen());
    pDoubles.write(event.getHigh());
    pDoubles.write(event.getLow());
    pDoubles.write(event.getClose());
    pDoubles.write(event.getVolume());
    pDoubles.write(event.getVWAP());
    pDoubles.write(event.getBidVolume());
    pDoubles.write(event.getAskVolume());
    pDoubles.write(event.getImpVolatility());
    pDoubles.write(event.getOpenInterest());
  }

  public static Candle fromNative(NativeEventsReader eventsReader) {
    Candle candle = new Candle();

    candle.setEventSymbol(CandleSymbol.valueOf(eventsReader.readString()));
    candle.setEventFlags(eventsReader.readInt());
    candle.setEventTime(eventsReader.readLong());
    candle.setIndex(eventsReader.readLong());
    candle.setCount(eventsReader.readLong());

    candle.setOpen(eventsReader.readDouble());
    candle.setHigh(eventsReader.readDouble());
    candle.setLow(eventsReader.readDouble());
    candle.setClose(eventsReader.readDouble());
    candle.setVolumeAsDouble(eventsReader.readDouble());
    candle.setVWAP(eventsReader.readDouble());
    candle.setBidVolumeAsDouble(eventsReader.readDouble());
    candle.setAskVolumeAsDouble(eventsReader.readDouble());
    candle.setImpVolatility(eventsReader.readDouble());
    candle.setOpenInterestAsDouble(eventsReader.readDouble());

    return candle;
  }
}
