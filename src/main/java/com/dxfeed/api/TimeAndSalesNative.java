package com.dxfeed.api;

import com.dxfeed.event.market.TimeAndSale;

import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

class TimeAndSalesNative {
    private static final ConcurrentHashMap<String, byte[]> stringToBytes = new ConcurrentHashMap<>();

    private int totalBytes = 0;
    private final ByteBuffer[] pBytes;
    private final DoubleBuffer pDouble;

    private static int writeBytes(byte[] data, int pos, byte[] value) {
        System.arraycopy(value, 0, data, pos, value.length);
        return pos + value.length;
    }

    private static int writeString(byte[] data, int pos, String value) {
        if (value == null || value.length() == 0) {
            return writeByte(data, pos, (byte) 0); // 0 as empty str
        }
        byte length = (byte) value.length();
        pos = writeByte(data, pos, length);
        byte[] strBytes = stringToBytes.computeIfAbsent(value, k -> value.getBytes(StandardCharsets.UTF_8));
        pos = writeBytes(data, pos, strBytes);
        return pos;
    }

    private static int writeByte(byte[] data, int pos, byte value) {
        data[pos] = value;
        ++pos;
        return pos;
    }

    private static int writeInt(byte[] data, int pos, int value) {
        data[pos++] = (byte) value;
        data[pos++] = (byte) (value >> 8);
        data[pos++] = (byte) (value >> 16);
        data[pos++] = (byte) (value >> 24);
        return pos;
    }

    private static int writeLong(byte[] data, int pos, long value) {
        for (int i = 0; i < 8; i++) {
            data[pos++] = (byte) value;
            value = value >> 8;
        }
        return pos;
    }

    private static int writeDouble(double[] data, int pos, double value) {
        data[pos++] = value;
        return pos;
    }


    TimeAndSalesNative(List<TimeAndSale> quoteList) {
        int quoteCount = quoteList.size();
        pBytes = new ByteBuffer[quoteCount];
        pDouble = DoubleBuffer.allocate(quoteCount * 3);
        int pos = 0;
        int doublePos = 0;
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

            pBytes[i] = ByteBuffer.allocate(totalSize);
            ByteBuffer buffer = pBytes[i];
            final byte[] array = buffer.array();
            pos = writeString(array, pos, eventSymbol);             // 1 + eventSymbolLength
            pos = writeLong(array, pos, eventTime);                 // 8
            pos = writeLong(array, pos, index);                     // 8
            pos = writeInt(array, pos, event_flags);                // 4
            pos = writeInt(array, pos, time_nano_part);             // 4
            pos = writeByte(array, pos, exchange_code);             // 1
            pos = writeLong(array, pos, quoteSize);                 // 8
            pos = writeString(array, pos, exchangeSaleConditions);  // 1 + exchangeSaleConditionsLength
            pos = writeString(array, pos, buyer);                   // 1 +  buyerLength
            pos = writeString(array, pos, seller);                  // 1 +  sellerLength
            pos = 0;
            totalBytes += totalSize;


            // DOUBLE DATA
            double[] doubleArray = pDouble.array();
            doubleArray[doublePos++] = quote.getPrice();
            doubleArray[doublePos++] = quote.getBidPrice();
            doubleArray[doublePos++] = quote.getAskPrice();
        }
    }

    public byte[] byteData() {
        ByteBuffer res = ByteBuffer.allocate(totalBytes);
        for (ByteBuffer buffer : pBytes) {
            res.put(buffer.array());
        }
        totalBytes = 0;
        return res.array();
    }

    public double[] doubleData() {
        return pDouble.array();
    }
}
