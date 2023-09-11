package com.dxfeed.api;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.serializers.*;
import com.dxfeed.event.EventType;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.market.*;
import com.dxfeed.event.misc.Configuration;
import com.dxfeed.event.misc.Message;
import com.dxfeed.event.option.Greeks;
import com.dxfeed.event.option.Series;
import com.dxfeed.event.option.TheoPrice;
import com.dxfeed.event.option.Underlying;

import java.util.ArrayList;
import java.util.List;

public class NativeEventsList<T extends EventType<?>> {
  private ByteBuffer pBytes;
  private DoubleBuffer pDoubles;
  public byte[] pEventTypes;

  NativeEventsList(List<T> eventList) {
    int eventCount = eventList.size();
    pBytes = new ByteBuffer(eventCount);
    pDoubles = new DoubleBuffer(eventCount);
    pEventTypes = new byte[eventCount];
    for (int i = 0; i < eventCount; ++i) {
      EventType<?> event = eventList.get(i);
      if (event instanceof TimeAndSale) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_TIME_AND_SALE;
        TimeAndSalesToNative.convert((TimeAndSale) event, pBytes, pDoubles);
      } else if (event instanceof Quote) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_QUOTE;
        QuoteToNative.convert((Quote) event, pBytes, pDoubles);
      } else if (event instanceof Candle) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_CANDLE;
        CandleToNative.convert((Candle) event, pBytes, pDoubles);
      } else if (event instanceof TradeBase) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_TRADE;
        TradeToNative.convert((TradeBase) event, pBytes, pDoubles);
      } else if (event instanceof Profile) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_PROFILE;
        ProfileToNative.convert((Profile) event, pBytes, pDoubles);
      } else if (event instanceof Summary) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_SUMMARY;
        SummaryToNative.convert((Summary) event, pBytes, pDoubles);
      } else if (event instanceof Greeks) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_GREEKS;
        GreeksToNative.convert((Greeks) event, pBytes, pDoubles);
      }else if (event instanceof Underlying) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_UNDERLYING;
        UnderlyingToNative.convert((Underlying) event, pBytes, pDoubles);
      } else if (event instanceof TheoPrice) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_THEO_PRICE;
        TheoPriceToNative.convert((TheoPrice) event, pBytes, pDoubles);
      } else if (event instanceof Configuration) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_CONFIGURATION;
        ConfigurationToNative.convert((Configuration) event, pBytes);
      } else if (event instanceof Message) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_MESSAGE;
        MessageToNative.convert((Message) event, pBytes);
      } else if (event instanceof OptionSale) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_OPTION_SALE;
        OptionSaleToNative.convert((OptionSale) event, pBytes, pDoubles);
      } else if (event instanceof OrderBase) {
        pEventTypes[i] = OrderToNative.convert((OrderBase) event, pBytes, pDoubles);
      } else if (event instanceof Series) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_SERIES;
        SeriesToNative.convert((Series) event, pBytes, pDoubles);
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

  public static List<EventType<?>> toList(byte[] byteData, double[] doubleData, byte[] eventTypes) {
    List<EventType<?>> eventList = new ArrayList<>();
    for (byte pEventType : eventTypes) {
      eventList.add(readEvent(pEventType, byteData, doubleData));
    }
    return eventList;
  }

  private static EventType<?> readEvent(byte pEventType, byte[] byteData, double[] doubleData) {
    switch (pEventType) {
      case DxfgEventClazzT.DXFG_EVENT_QUOTE: {
        return QuoteToNative.fromNative(byteData, doubleData);
      }
      case DxfgEventClazzT.DXFG_EVENT_CANDLE: {
        return CandleToNative.fromNative(byteData, doubleData);
      }
      case DxfgEventClazzT.DXFG_EVENT_TRADE: {
        return TradeToNative.fromNative(byteData, doubleData);
      }
      case DxfgEventClazzT.DXFG_EVENT_PROFILE: {
        return ProfileToNative.fromNative(byteData, doubleData);
      }
      default:
        throw new IllegalStateException("Event mapping for event type " + pEventType + " is not implemented");
    }
  }

  public byte[] byteData() {
    return pBytes.toByteData();
  }

  public double[] doubleData() {
    return pDoubles.toDoubleData();
  }
}
