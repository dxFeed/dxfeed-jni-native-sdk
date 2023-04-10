package com.dxfeed.api;

import com.dxfeed.event.market.TimeAndSale;

import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

class TimeAndSalesNative {
    private static final ConcurrentHashMap<String, byte[]> stringToBytes = new ConcurrentHashMap<>();
    private final ByteBufferWrapper pBytes;
    private final DoubleBufferWrapper pDoubles;

    TimeAndSalesNative(List<TimeAndSale> quoteList) {
        int quoteCount = quoteList.size();
        pBytes = new ByteBufferWrapper(quoteCount, stringToBytes);
        pDoubles = new DoubleBufferWrapper(quoteCount, 3);
        for (int i = 0; i < quoteCount; ++i) {
            TimeAndSale quote = quoteList.get(i);
            String eventSymbol = quote.getEventSymbol();
            int eventSymbolLength = eventSymbol.length();
            long eventTime = quote.getEventTime();                                   // 8
            long index = quote.getIndex();                                           // 8
            int event_flags = quote.getEventFlags();                                 // 4
            int time_nano_part = quote.getTimeNanoPart();                            // 4
            byte exchange_code = (byte) quote.getExchangeCode();                     // 1
            long quoteSize = quote.getSize();                                        // 8

            int totalSize = (4 + eventSymbolLength) + (8) + (8) + (4) + (4) + (1) + (8) + (4 + 4 + 4);
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

            pBytes.addChunk(i, totalSize);
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

            // DOUBLE DATA
            pDoubles.write(quote.getPrice());
            pDoubles.write(quote.getBidPrice());
            pDoubles.write(quote.getAskPrice());
        }
    }

    public byte[] byteData() {
        return pBytes.toByteData();
    }

    public double[] doubleData() {
        return pDoubles.toDoubleData();
    }
}
