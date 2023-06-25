package com.dxfeed.api;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.api.serializers.CandleToNative;
import com.dxfeed.api.serializers.QuoteToNative;
import com.dxfeed.api.serializers.TimeAndSalesToNative;
import com.dxfeed.event.EventType;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.market.Quote;
import com.dxfeed.event.market.TimeAndSale;

import java.util.List;

public class NativeEventsList {
    private ChunkedByteBuffer pBytes;
    private ChunkedDoubleBuffer pDoubles;
    public byte[] pEventTypes;

    NativeEventsList(List<EventType<?>> eventList) {
        int eventCount = eventList.size();
        pBytes = new ChunkedByteBuffer(eventCount);
        pDoubles = new ChunkedDoubleBuffer(eventCount);
        pEventTypes = new byte[eventCount];
        for (int i = 0; i < eventCount; ++i) {
            EventType<?> event = eventList.get(i);
            if (event instanceof TimeAndSale) {
                pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_TIME_AND_SALE.eventOrdinal();
                TimeAndSalesToNative.convert((TimeAndSale) event, pBytes, pDoubles, i);
            } else if (event instanceof Quote) {
                pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_QUOTE.eventOrdinal();
                QuoteToNative.convert((Quote) event, pBytes, pDoubles, i);
            } else if (event instanceof Candle) {
                pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_CANDLE.eventOrdinal();
                CandleToNative.convert((Candle) event, pBytes, pDoubles, i);
            }
        }
    }

    public void clear() {
        pBytes.clear();
        pBytes = null;
        pDoubles.clear();
        pDoubles = null;
        pEventTypes = null;
    }

    public byte[] byteData() {
        return pBytes.toByteData();
    }

    public double[] doubleData() {
        return pDoubles.toDoubleData();
    }
}
