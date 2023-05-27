package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.candle.Candle;

public class CandleToNative {
    /**
     * typedef struct dxfg_market_event_t {
     *     dxfg_event_type_t event_type;
     *     const char *event_symbol;
     *     int64_t event_time;
     * } dxfg_market_event_t;
     * // https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L219
     * typedef struct dxfg_candle_t {
     *     dxfg_event_type_t event_type;
     *     //  dxfg_time_series_event_t time_series_event;
     *     //  dxfg_lasting_event_t lasting_event;
     *     int32_t event_flags;
     *     int64_t index;
     *     int64_t count;
     *     double open;
     *     double high;
     *     double low;
     *     double close;
     *     double volume;
     *     double vwap;
     *     double bid_volume;
     *     double ask_volume;
     *     double imp_volatility;
     *     double open_interest;
     * } dxfg_candle_t;
     */

    // todo: sync about CandleSymbol
    public static void convert(Candle event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
//        CandleSymbol candleSymbol = event.getEventSymbol();
//        String eventSymbol = candleSymbol.toString();
//        int eventSymbolLength = eventSymbol.length();
        long eventTime = event.getEventTime();          // 8
        int eventFlags = event.getEventFlags();         // 4
        long index = event.getIndex();                  // 8
        long count = event.getCount();                  // 8
        long volume = event.getVolume();                // 8
        long bidVolume = event.getBidVolume();          // 8
        long askVolume = event.getAskVolume();          // 8
        long openInterest = event.getOpenInterest();    // 8

        int totalSize = /* (2 + eventSymbolLength) */ + (8) + (4) + (8) + (8) + (8) + (8) + (8) + (8);

        pBytes.addChunk(chunkIdx, totalSize);
//        pBytes.writeString(eventSymbol);        // 2 + eventSymbolLength
        pBytes.writeLong(eventTime);            // 8
        pBytes.writeInt(eventFlags);            // 4
        pBytes.writeLong(index);                // 8
        pBytes.writeLong(count);                // 8
        pBytes.writeLong(volume);               // 8
        pBytes.writeLong(bidVolume);            // 8
        pBytes.writeLong(askVolume);            // 8
        pBytes.writeLong(openInterest);         // 8

        // DOUBLE DATA
        double open = event.getOpen();                      // 1
        double high = event.getHigh();                      // 1
        double low = event.getLow();                        // 1
        double close = event.getClose();                    // 1
        double vwap = event.getVWAP();                      // 1
        double impVolatility = event.getImpVolatility();    // 1
        // DOUBLE DATA
        pDoubles.addChunk(chunkIdx, 6);
        pDoubles.write(open);
        pDoubles.write(high);
        pDoubles.write(low);
        pDoubles.write(close);
        pDoubles.write(vwap);
        pDoubles.write(impVolatility);
    }
}
