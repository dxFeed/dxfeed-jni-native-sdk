package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.DxFeedEventMarket;
import com.dxfeed.event.market.Quote;

public class QuoteToNative {
    /**
     * typedef struct dxfg_market_event_t {
     *   dxfg_event_type_t event_type;
     *   const char *event_symbol;
     *   int64_t event_time;
     * } dxfg_market_event_t;
     *
     * // https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L140
     * typedef struct dxfg_quote_t {
     *   dxfg_market_event_t market_event;
     *   int32_t time_millis_sequence;
     *   int32_t time_nano_part;
     *   int64_t bid_time;
     *   int16_t bid_exchange_code;
     *   double bid_price;
     *   double bid_size;
     *   int64_t ask_time;
     *   int16_t ask_exchange_code;
     *   double ask_price;
     *   double ask_size;
     * } dxfg_quote_t;
     */
    public static void convert(Quote event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
        CString eventSymbol = new CString(event.getEventSymbol());
        long eventTime = event.getEventTime();                                                          // 8
        int timeMillisSequence = DxFeedEventMarket.QuotePackagePrivate.getTimeMillisSequence(event);    // 4
        int timeNanoPart = event.getTimeNanoPart();                                                     // 4
        long bidTime = event.getBidTime();                                                              // 8
        char bidExchangeCode = event.getBidExchangeCode();                                              // 2
        long askTime = event.getAskTime();                                                              // 8
        char askExchangeCode = event.getAskExchangeCode();                                              // 2

        int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (4) + (8) + (2) + (8) + (2);

        pBytes.addChunk(chunkIdx, totalSize);
        pBytes.writeString(eventSymbol);            // 2 + eventSymbolLength
        pBytes.writeLong(eventTime);                // 8
        pBytes.writeInt(timeMillisSequence);        // 4
        pBytes.writeInt(timeNanoPart);              // 4
        pBytes.writeLong(bidTime);                  // 8
        pBytes.writeChar(bidExchangeCode);          // 2
        pBytes.writeLong(askTime);                  // 8
        pBytes.writeChar(askExchangeCode);          // 2

        // DOUBLE DATA
        double bidPrice = event.getBidPrice();  // 1
        double bidSize = event.getBidSize();    // 1
        double askPrice = event.getAskPrice();  // 1
        double askSize = event.getAskSize();    // 1

        // DOUBLE DATA
        pDoubles.addChunk(chunkIdx, 4);
        pDoubles.write(bidPrice);
        pDoubles.write(bidSize);
        pDoubles.write(askPrice);
        pDoubles.write(askSize);
    }
}
