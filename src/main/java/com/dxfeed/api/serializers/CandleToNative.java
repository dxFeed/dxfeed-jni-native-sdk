package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.candle.CandleSymbol;

public class CandleToNative {
  /**
   * typedef struct dxfg_candle_symbol_t {
   * dxfg_symbol_t supper;
   * const char* symbol;
   * } dxfg_candle_symbol_t;
   * <p>
   * // https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L219
   * typedef struct dxfg_candle_t {
   * dxfg_event_type_t event_type;
   * //  dxfg_time_series_event_t time_series_event;
   * //  dxfg_lasting_event_t lasting_event;
   * dxfg_candle_symbol_t *event_symbol;
   * int32_t event_flags;
   * int64_t event_time;
   * int64_t index;
   * int64_t count;
   * double open;
   * double high;
   * double low;
   * double close;
   * double volume;
   * double vwap;
   * double bid_volume;
   * double ask_volume;
   * double imp_volatility;
   * double open_interest;
   * } dxfg_candle_t;
   */

  // todo: sync about CandleSymbol
  public static void convert(Candle event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    CandleSymbol candleSymbol = event.getEventSymbol();
    CString eventSymbol = new CString(candleSymbol.toString());
    int eventFlags = event.getEventFlags();         // 4
    long eventTime = event.getEventTime();          // 8
    long index = event.getIndex();                  // 8
    long count = event.getCount();                  // 8

    int totalSize = eventSymbol.totalAllocatedBytes + (4) + (8) + (8) + (8);

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeInt(eventFlags);            // 4
    pBytes.writeLong(eventTime);            // 8
    pBytes.writeLong(index);                // 8
    pBytes.writeLong(count);                // 8

    // DOUBLE DATA
    double open = event.getOpen();                      // 1
    double high = event.getHigh();                      // 1
    double low = event.getLow();                        // 1
    double close = event.getClose();                    // 1
    double volume = event.getVolume();                  // 1
    double vwap = event.getVWAP();                      // 1
    double bidVolume = event.getBidVolume();            // 1
    double askVolume = event.getAskVolume();            // 1
    double impVolatility = event.getImpVolatility();    // 1
    double openInterest = event.getOpenInterest();      // 1

    // DOUBLE DATA
    pDoubles.addChunk(chunkIdx, 10);
    pDoubles.write(open);
    pDoubles.write(high);
    pDoubles.write(low);
    pDoubles.write(close);
    pDoubles.write(volume);
    pDoubles.write(vwap);
    pDoubles.write(bidVolume);
    pDoubles.write(askVolume);
    pDoubles.write(impVolatility);
    pDoubles.write(openInterest);
  }
}
