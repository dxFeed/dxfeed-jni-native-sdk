package com.dxfeed.api;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
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
        TimeAndSalesMapping.toNative((TimeAndSale) event, pBytes, pDoubles);
      } else if (event instanceof Quote) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_QUOTE;
        QuoteMapping.toNative((Quote) event, pBytes, pDoubles);
      } else if (event instanceof Candle) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_CANDLE;
        CandleMapping.toNative((Candle) event, pBytes, pDoubles);
      } else if (event instanceof TradeBase) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_TRADE;
        TradeMapping.toNative((TradeBase) event, pBytes, pDoubles);
      } else if (event instanceof Profile) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_PROFILE;
        ProfileMapping.toNative((Profile) event, pBytes, pDoubles);
      } else if (event instanceof Summary) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_SUMMARY;
        SummaryMapping.toNative((Summary) event, pBytes, pDoubles);
      } else if (event instanceof Greeks) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_GREEKS;
        GreeksMapping.toNative((Greeks) event, pBytes, pDoubles);
      }else if (event instanceof Underlying) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_UNDERLYING;
        UnderlyingMapping.toNative((Underlying) event, pBytes, pDoubles);
      } else if (event instanceof TheoPrice) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_THEO_PRICE;
        TheoPriceMapping.toNative((TheoPrice) event, pBytes, pDoubles);
      } else if (event instanceof Configuration) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_CONFIGURATION;
        ConfigurationMapping.toNative((Configuration) event, pBytes);
      } else if (event instanceof Message) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_MESSAGE;
        MessageMapping.toNative((Message) event, pBytes);
      } else if (event instanceof OptionSale) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_OPTION_SALE;
        OptionSaleMapping.toNative((OptionSale) event, pBytes, pDoubles);
      } else if (event instanceof OrderBase) {
        pEventTypes[i] = OrderToMapping.toNative((OrderBase) event, pBytes, pDoubles);
      } else if (event instanceof Series) {
        pEventTypes[i] = DxfgEventClazzT.DXFG_EVENT_SERIES;
        SeriesMapping.toNative((Series) event, pBytes, pDoubles);
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
    NativeEventsReader reader = new NativeEventsReader(byteData, doubleData);
    for (byte pEventType : eventTypes) {
      eventList.add(readEvent(reader, pEventType));
    }
    return eventList;
  }

  private static EventType<?> readEvent(NativeEventsReader reader, byte pEventType) {
    switch (pEventType) {
      case DxfgEventClazzT.DXFG_EVENT_QUOTE: {
        return QuoteMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_PROFILE: {
        return ProfileMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_SUMMARY: {
        return SummaryMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_GREEKS: {
        return GreeksMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_CANDLE: {
        return CandleMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_UNDERLYING: {
        return UnderlyingMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_THEO_PRICE: {
        return TheoPriceMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_TRADE:
      case DxfgEventClazzT.DXFG_EVENT_TRADE_ETH: {
        return TradeMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_TIME_AND_SALE: {
        return TimeAndSalesMapping.fromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_ORDER_BASE: {
        return OrderToMapping.orderBaseFromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_ORDER: {
        return OrderToMapping.orderFromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_ANALYTIC_ORDER: {
        return OrderToMapping.analyticOrderFromNative(reader);
      }
      case DxfgEventClazzT.DXFG_EVENT_SPREAD_ORDER: {
        return OrderToMapping.spreadOrderFromNative(reader);
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
