package com.dxfeed.api;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.api.serializers.*;
import com.dxfeed.event.EventType;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.market.*;
import com.dxfeed.event.misc.Configuration;
import com.dxfeed.event.option.TheoPrice;
import com.dxfeed.event.option.Underlying;

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
      } else if (event instanceof TradeBase) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_TRADE.eventOrdinal();
        TradeToNative.convert((TradeBase) event, pBytes, pDoubles, i);
      } else if (event instanceof Profile) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_PROFILE.eventOrdinal();
        ProfileToNative.convert((Profile) event, pBytes, pDoubles, i);
      } else if (event instanceof Underlying) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_UNDERLYING.eventOrdinal();
        UnderlyingToNative.convert((Underlying) event, pBytes, pDoubles, i);
      } else if (event instanceof TheoPrice) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_THEO_PRICE.eventOrdinal();
        TheoPriceToNative.convert((TheoPrice) event, pBytes, pDoubles, i);
      } else if (event instanceof Configuration) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_CONFIGURATION.eventOrdinal();
        ConfigurationToNative.convert((Configuration) event, pBytes, i);
      } else {
        throw new IllegalStateException("Event mapping for " + event.getClass().getName() + " is not implemented");
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
