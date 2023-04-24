package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.TimeAndSale;

public class TimeAndSalesToNative {
    public static void convert(TimeAndSale quote, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
        String eventSymbol = quote.getEventSymbol();
        int eventSymbolLength = eventSymbol.length();
        long eventTime = quote.getEventTime();                                   // 8
        long index = quote.getIndex();                                           // 8
        int event_flags = quote.getEventFlags();                                 // 4
        int time_nano_part = quote.getTimeNanoPart();                            // 4
        byte exchange_code = (byte) quote.getExchangeCode();                     // 1
        long quoteSize = quote.getSize();                                        // 8

        int totalSize = (2 + eventSymbolLength) + (8) + (8) + (4) + (4) + (1) + (8) + (2 + 2 + 2);
        String exchangeSaleConditions = quote.getExchangeSaleConditions();
        if (exchangeSaleConditions != null) {
            totalSize += exchangeSaleConditions.length();
        }
        String buyer = quote.getBuyer();
        if (buyer != null) {
            totalSize += buyer.length();
        }
        String seller = quote.getSeller();
        if (seller != null) {
            totalSize += seller.length();
        }

        pBytes.addChunk(eventIdx, totalSize);
        pBytes.writeString(eventSymbol);             // 4 + eventSymbolLength
        pBytes.writeLong(eventTime);                 // 8
        pBytes.writeLong(index);                     // 8
        pBytes.writeInt(event_flags);                // 4
        pBytes.writeInt(time_nano_part);             // 4
        pBytes.writeByte(exchange_code);             // 1
        pBytes.writeLong(quoteSize);                 // 8
        pBytes.writeString(exchangeSaleConditions);  // 4 + exchangeSaleConditionsLength
        pBytes.writeString(buyer);                   // 4 +  buyerLength
        pBytes.writeString(seller);                  // 4 +  sellerLength

        double price = quote.getPrice();        // 1
        double bidPrice = quote.getBidPrice();  // 1
        double askPrice = quote.getAskPrice();  // 1
        // DOUBLE DATA
        pDoubles.addChunk(eventIdx, 3);
        pDoubles.write(price);
        pDoubles.write(bidPrice);
        pDoubles.write(askPrice);
    }
}
