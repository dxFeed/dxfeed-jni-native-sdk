package com.dxfeed.api;

import com.dxfeed.api.osub.WildcardSymbol;
import com.dxfeed.event.EventType;
import com.dxfeed.event.candle.CandleSymbol;

import java.util.concurrent.ConcurrentHashMap;
import java.util.function.BiConsumer;

public class DxSubscriptionJni {
    private static final ConcurrentHashMap<Long, DXFeedEventListener<EventType<?>>> eventListenerMap = new ConcurrentHashMap<>();

    // callbacks from native
    private static long newEventListener(long userCallback, long userData) {
        long id = DxFeedJni.nextHandleId();
        System.out.println("DxSubscriptionJni::newEventListener, nativeHandle = " + id);
        eventListenerMap.put(id, eventList -> {
            EventsNative nativeTS = new EventsNative(eventList);
            nOnEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
                    userCallback, userData);
            nativeTS.clear();
        });
        return id;
    }

    private static void addEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
        DXFeedEventListener<EventType<?>> eventListener = eventListenerMap.get(nativeHandleId);
        if (eventListener != null) {
            System.out.println("DxSubscriptionJni::addEventListener, nativeHandle = " + nativeHandleId);
            sub.addEventListener(eventListener);
        }
    }

    private static void removeEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
        DXFeedEventListener<EventType<?>> eventListener = eventListenerMap.remove(nativeHandleId);
        if (eventListener != null) {
            System.out.println("DxSubscriptionJni::removeEventListener, nativeHandle = " + nativeHandleId);
            sub.removeEventListener(eventListener);
        }
    }

    private static void setSymbol(DXFeedSubscription<EventType<?>> sub, int symbolType, String symbol) {
        processSymbol(sub, symbolType, symbol, sub::setSymbols);
    }

    private static void addSymbol(DXFeedSubscription<EventType<?>> sub, int symbolType, String symbol) {
        processSymbol(sub, symbolType, symbol, sub::addSymbols);
    }

    private static void processSymbol(DXFeedSubscription<EventType<?>> sub, int symbolType, String symbol,
                                      DxSymbolConsumer symbolConsumer)
    {
        if (symbolType == DxfgSymbolType.STRING.ordinal()) {
            System.out.println("DxSubscriptionJni::setSymbol, symbolType = " + DxfgSymbolType.STRING +
                    ", symbol = " + symbol);
            symbolConsumer.accept(sub, symbol);
        } else if (symbolType == DxfgSymbolType.CANDLE.ordinal()) {
            CandleSymbol candleSymbol = CandleSymbol.valueOf(symbol);
            System.out.println("DxSubscriptionJni::setSymbol, symbolType = " + DxfgSymbolType.CANDLE +
                    ", symbol = " + candleSymbol);
            symbolConsumer.accept(sub, candleSymbol);
        } else if (symbolType == DxfgSymbolType.WILDCARD.ordinal()) {
            System.out.println("DxSubscriptionJni::setSymbol, symbolType = " + DxfgSymbolType.WILDCARD +
                    ", symbol = " + WildcardSymbol.ALL);
            symbolConsumer.accept(sub, WildcardSymbol.ALL);
        } else {
            throw new IllegalStateException();
        }
    }

    private interface DxSymbolConsumer extends BiConsumer<DXFeedSubscription<EventType<?>>, Object> {}

    private static native void nOnEventListener(int size, byte[] byteData, double[] doubleData,
                                                byte[] pEventTypes, long userCallback, long userData);
}