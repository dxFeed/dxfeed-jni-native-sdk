package com.dxfeed.api;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.api.serializers.QuoteToNative;
import com.dxfeed.api.serializers.TimeAndSalesToNative;
import com.dxfeed.event.EventType;
import com.dxfeed.event.market.Quote;
import com.dxfeed.event.market.TimeAndSale;

import java.util.List;

public class EventsNative {
    private final ChunkedByteBuffer pBytes;
    private final ChunkedDoubleBuffer pDoubles;
    public final byte[] pEventTypes;

    EventsNative(List<EventType<?>> quoteList) {
        int quoteCount = quoteList.size();
        pBytes = new ChunkedByteBuffer(quoteCount);
        pDoubles = new ChunkedDoubleBuffer(quoteCount);
        pEventTypes = new byte[quoteCount];
        for (int i = 0; i < quoteCount; ++i) {
            EventType<?> quote = quoteList.get(i);
            if (quote instanceof TimeAndSale) {
                pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_TIME_AND_SALE.eventOrdinal();
                TimeAndSalesToNative.convert((TimeAndSale) quote, pBytes, pDoubles, i);
            } else  if (quote instanceof Quote) {
                pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_QUOTE.eventOrdinal();
                QuoteToNative.convert((Quote) quote, pBytes, pDoubles, i);
            }
        }
    }

    public byte[] byteData() {
        return pBytes.toByteData();
    }

    public double[] doubleData() {
        return pDoubles.toDoubleData();
    }
}
