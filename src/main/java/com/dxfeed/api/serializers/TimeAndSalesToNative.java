package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.DxFeedEventMarket;
import com.dxfeed.event.market.TimeAndSale;

public class TimeAndSalesToNative {
    /**
     * typedef struct dxfg_market_event_t {
     *     dxfg_event_type_t event_type;
     *     const char *event_symbol;
     *     int64_t event_time;
     * } dxfg_market_event_t;
     *
     * typedef struct dxfg_time_and_sale_t {
     *     dxfg_market_event_t market_event;
     *     int32_t event_flags;
     *     int64_t index;
     *     int32_t time_nano_part;
     *     int16_t exchange_code;
     *     double price;
     *     double size;
     *     double bid_price;
     *     double ask_price;
     *     const char *exchange_sale_conditions;
     *     int32_t flags;
     *     const char *buyer;
     *     const char *seller;
     * } dxfg_time_and_sale_t;
     */

    public static void convert(TimeAndSale event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
        String eventSymbol = event.getEventSymbol();
        int eventSymbolLength = eventSymbol.length();
        long eventTime = event.getEventTime();                                      // 8
        int eventFlags = event.getEventFlags();                                     // 4
        long index = event.getIndex();                                              // 8
        int timeNanoPart = event.getTimeNanoPart();                                 // 4
        char exchangeCode = event.getExchangeCode();                                // 2
        long quoteSize = event.getSize();                                           // 8
        int flags = DxFeedEventMarket.TimeAndSalePackagePrivate.getFlags(event);    // 4

        int totalSize = (2 + eventSymbolLength) + (8) + (4) + (8) + (4) + (2) + (8) + (4) + (2 + 2 + 2);
        String exchangeSaleConditions = event.getExchangeSaleConditions();
        if (exchangeSaleConditions != null) {
            totalSize += exchangeSaleConditions.length();
        }
        String buyer = event.getBuyer();
        if (buyer != null) {
            totalSize += buyer.length();
        }
        String seller = event.getSeller();
        if (seller != null) {
            totalSize += seller.length();
        }

        pBytes.addChunk(eventIdx, totalSize);
        pBytes.writeString(eventSymbol);            // 2 + eventSymbolLength
        pBytes.writeLong(eventTime);                // 8
        pBytes.writeInt(eventFlags);                // 4
        pBytes.writeLong(index);                    // 8
        pBytes.writeInt(timeNanoPart);              // 4
        pBytes.writeChar(exchangeCode);             // 2
        pBytes.writeLong(quoteSize);                // 8
        pBytes.writeInt(flags);                     // 4
        pBytes.writeString(exchangeSaleConditions); // 2 + exchangeSaleConditionsLength
        pBytes.writeString(buyer);                  // 2 +  buyerLength
        pBytes.writeString(seller);                 // 2 +  sellerLength

        double price = event.getPrice();        // 1
        double bidPrice = event.getBidPrice();  // 1
        double askPrice = event.getAskPrice();  // 1
        // DOUBLE DATA
        pDoubles.addChunk(eventIdx, 3);
        pDoubles.write(price);
        pDoubles.write(bidPrice);
        pDoubles.write(askPrice);
    }
}
