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
            byte eventSymbolLength = (byte) eventSymbol.length();
            long eventTime = quote.getEventTime();                                   // 8
            long index = quote.getIndex();                                           // 8
            int event_flags = quote.getEventFlags();                                 // 4
            int time_nano_part = quote.getTimeNanoPart();                            // 4
            byte exchange_code = (byte) quote.getExchangeCode();                     // 1
            long quoteSize = quote.getSize();                                        // 8

            int totalSize = (1 + eventSymbolLength) + (8) + (8) + (4) + (4) + (1) + (8) + (1 + 1 + 1);
            String exchangeSaleConditions = quote.getExchangeSaleConditions();
            if (exchangeSaleConditions != null) {
                totalSize += (byte) exchangeSaleConditions.length();
            }
            String buyer = quote.getBuyer();
            if (buyer != null) {
                totalSize += (byte) buyer.length();
            }
            String seller = quote.getSeller();
            if (seller != null) {
                totalSize += (byte) seller.length();
            }

            pBytes.addChunk(i, totalSize);
            pBytes.writeString(eventSymbol);             // 1 + eventSymbolLength
            pBytes.writeLong(eventTime);                 // 8
            pBytes.writeLong(index);                     // 8
            pBytes.writeInt(event_flags);                // 4
            pBytes.writeInt(time_nano_part);             // 4
            pBytes.writeByte(exchange_code);             // 1
            pBytes.writeLong(quoteSize);                 // 8
            pBytes.writeString(exchangeSaleConditions);  // 1 + exchangeSaleConditionsLength
            pBytes.writeString(buyer);                   // 1 +  buyerLength
            pBytes.writeString(seller);                  // 1 +  sellerLength

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
