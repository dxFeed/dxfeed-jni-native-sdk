package com.dxfeed.api;

import com.dxfeed.event.EventType;

import java.util.Iterator;
import java.util.Set;

public class DxSubscriptionJni {
  // callbacks from native
  private static long newEventListener(long userCallback, long userData) {
    long id = DxFeedJni.nextHandleId();
    System.out.println("DxSubscriptionJni::newEventListener, nativeHandle = " + id);
    DXFeedEventListener<EventType<?>> listener = eventList -> {
      NativeEventsList<EventType<?>> nativeTS = new NativeEventsList<>(eventList);
      nOnEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
          userCallback, userData);
      nativeTS.clear();
    };
    DxFeedJni.nativeObjectsMap.put(id, listener);
    return id;
  }

  private static void addEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
    DXFeedEventListener<EventType<?>> eventListener =
        (DXFeedEventListener<EventType<?>>) DxFeedJni.nativeObjectsMap.get(nativeHandleId);
    if (eventListener != null) {
      System.out.println("DxSubscriptionJni::addEventListener, nativeHandle = " + nativeHandleId);
      sub.addEventListener(eventListener);
    }
  }

  private static void removeEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
    DXFeedEventListener<EventType<?>> eventListener =
        (DXFeedEventListener<EventType<?>>) DxFeedJni.nativeObjectsMap.remove(nativeHandleId);
    if (eventListener != null) {
      System.out.println("DxSubscriptionJni::removeEventListener, nativeHandle = " + nativeHandleId);
      sub.removeEventListener(eventListener);
    }
  }

  private static byte[] getEventTypes(DXFeedSubscription<EventType<?>> sub) {
    Set<Class<? extends EventType<?>>> eventTypes = sub.getEventTypes();
    Iterator<Class<? extends EventType<?>>> iterator = eventTypes.iterator();
    int eventsCount = eventTypes.size();
    byte[] pEventTypes = new byte[eventsCount];
    for (int i = 0; i < eventsCount && iterator.hasNext(); ++i) {
      pEventTypes[i] = DxfgEventClazzT.fromClass(iterator.next());
    }
    return pEventTypes;
  }

  private static void getSymbols(DXFeedSubscription<EventType<?>> sub) {
    Set<?> eventTypes = sub.getSymbols();
    Iterator<?> iterator = eventTypes.iterator();
    while (iterator.hasNext()) {
//      Object jObject = iterator.next();
//      if (jObject instanceof String) {
//        final DxfgStringSymbol dxfgStringSymbol = (DxfgStringSymbol) nObject;
//        final String stringSymbol = (String) jObject;
//        dxfgStringSymbol.setType(DxfgSymbolType.STRING.getCValue());
//        dxfgStringSymbol.setSymbol(this.stringMapper.toNative(stringSymbol));
//      } else if (jObject instanceof CandleSymbol) {
//        final DxfgCandleSymbol dxfgCandleSymbol = (DxfgCandleSymbol) nObject;
//        final CandleSymbol candleSymbol = (CandleSymbol) jObject;
//        dxfgCandleSymbol.setType(DxfgSymbolType.CANDLE.getCValue());
//        dxfgCandleSymbol.setSymbol(this.stringMapper.toNative(candleSymbol.toString()));
//      } else if (jObject instanceof WildcardSymbol) {
//        nObject.setType(DxfgSymbolType.WILDCARD.getCValue());
//      } else if (jObject instanceof TimeSeriesSubscriptionSymbol) {
//        final DxfgTimeSeriesSubscriptionSymbol dxfgTssSymbol = (DxfgTimeSeriesSubscriptionSymbol) nObject;
//        final TimeSeriesSubscriptionSymbol<?> tssSymbol = (TimeSeriesSubscriptionSymbol<?>) jObject;
//        dxfgTssSymbol.setType(DxfgSymbolType.TIME_SERIES_SUBSCRIPTION.getCValue());
//        dxfgTssSymbol.setSymbol(toNative(tssSymbol.getEventSymbol()));
//        dxfgTssSymbol.setFromTime(tssSymbol.getFromTime());
//      } else if (jObject instanceof IndexedEventSubscriptionSymbol) {
//        final DxfgIndexedEventSubscriptionSymbol dxfgIesSymbol = (DxfgIndexedEventSubscriptionSymbol) nObject;
//        final IndexedEventSubscriptionSymbol<?> iesSymbol = (IndexedEventSubscriptionSymbol<?>) jObject;
//        dxfgIesSymbol.setType(DxfgSymbolType.INDEXED_EVENT_SUBSCRIPTION.getCValue());
//        dxfgIesSymbol.setSymbol(toNative(iesSymbol.getEventSymbol()));
//        dxfgIesSymbol.setSource(this.mapperSource.toNative(iesSymbol.getSource()));
//      }
    }
  }

  private static native void nOnEventListener(int size, byte[] byteData, double[] doubleData,
                                              byte[] pEventTypes, long userCallback, long userData);
}
