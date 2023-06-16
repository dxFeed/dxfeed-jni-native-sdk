package com.dxfeed.api;

import com.dxfeed.api.osub.WildcardSymbol;
import com.dxfeed.event.EventType;
import com.dxfeed.event.LastingEvent;
import com.dxfeed.event.candle.CandleSymbol;

import java.beans.PropertyChangeListener;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;

public class DxFeedJni {
    static {
        String property = System.getProperty("com.devexperts.qd.impl.matrix.Agent.MaxBufferSize");
        System.out.println("[DxFeedJni]: After loading class " + DxFeedJni.class.getName() +
                ", com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = " + property);
    }

    private static final ConcurrentHashMap<Long, PropertyChangeListener> changeListenerMap = new ConcurrentHashMap<>();
    private static final ConcurrentHashMap<Long, DXFeedEventListener<EventType<?>>> eventListenerMap = new ConcurrentHashMap<>();
    private static final ConcurrentHashMap<Long, LastingEvent<?>> lastingEventMap = new ConcurrentHashMap<>();
    private static final ConcurrentHashMap<Long, Object> symbolMap = new ConcurrentHashMap<>();
    private static final AtomicLong nativeHandleId = new AtomicLong();

    // callbacks from native
    private static long newStateChangeEventListener(long userCallback, long userData) {
        long id = nativeHandleId.incrementAndGet();
        changeListenerMap.put(id, event -> nOnStateChangeListener(
                ((DXEndpoint.State) event.getOldValue()).ordinal(),
                ((DXEndpoint.State) event.getNewValue()).ordinal(),
                userCallback, userData));
        return id;
    }

    private static long newEventListener(long userCallback, long userData) {
        long id = nativeHandleId.incrementAndGet();
        eventListenerMap.put(id, eventList -> {
            EventsNative nativeTS = new EventsNative(eventList);
            nOnEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
                    userCallback, userData);
            nativeTS.clear();
        });
        return id;
    }

    private static void addStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
        PropertyChangeListener propertyChangeListener = changeListenerMap.get(nativeHandleId);
        if (propertyChangeListener != null) {
            System.out.println("addStateChangeEventListener, nativeHandle = " + nativeHandleId);
            endpoint.addStateChangeListener(propertyChangeListener);
        }
    }

    private static void addEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
        DXFeedEventListener<EventType<?>> eventListener = eventListenerMap.get(nativeHandleId);
        if (eventListener != null) {
            System.out.println("addEventListener, nativeHandle = " + nativeHandleId);
            sub.addEventListener(eventListener);
        }
    }

    private static void removeStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
        PropertyChangeListener stateChangeListener = changeListenerMap.remove(nativeHandleId);
        if (stateChangeListener != null) {
            System.out.println("removeStateChangeEventListener, nativeHandle = " + nativeHandleId);
            endpoint.removeStateChangeListener(stateChangeListener);
        }
    }

    private static void removeEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
        DXFeedEventListener<EventType<?>> eventListener = eventListenerMap.remove(nativeHandleId);
        if (eventListener != null) {
            System.out.println("removeEventListener, nativeHandle = " + nativeHandleId);
            sub.removeEventListener(eventListener);
        }
    }

    private static long newSymbol(String symbol, int symbolType) {
        boolean isTimeSeries = symbolType == DxfgSymbolType.TIME_SERIES_SUBSCRIPTION.ordinal();
        boolean isIndexedEvent = symbolType == DxfgSymbolType.INDEXED_EVENT_SUBSCRIPTION.ordinal();
        if (isTimeSeries || isIndexedEvent) {
            throw new IllegalStateException();
        }
        long id = nativeHandleId.incrementAndGet();
        if (symbolType == DxfgSymbolType.STRING.ordinal()) {
            symbolMap.put(id, symbol);
        } else if (symbolType == DxfgSymbolType.CANDLE.ordinal()) {
            CandleSymbol candleSymbol = CandleSymbol.valueOf(symbol);
            symbolMap.put(id, candleSymbol);
        } else if (symbolType == DxfgSymbolType.WILDCARD.ordinal()) {
            symbolMap.put(id, WildcardSymbol.ALL);
        } else {
            throw new IllegalStateException();
        }
        return id;
    }

    private static void releaseSymbol(long nativeHandlerId) {
        symbolMap.remove(nativeHandlerId);
    }

    private static <E extends LastingEvent<?>> long newEvent(Class<E> eventTypeClass, String symbol) {
        long id = nativeHandleId.incrementAndGet();
        try {
            Constructor<E> constructor = eventTypeClass.getConstructor(String.class);
            System.out.println("constructor = " + constructor);
            E e = constructor.newInstance(symbol);
            System.out.println("event = " + e);
            lastingEventMap.put(id, e);
            return id;
        } catch (NoSuchMethodException | InstantiationException | IllegalAccessException | InvocationTargetException e) {
            throw new RuntimeException(e);
        }
    }

    private static <E extends LastingEvent<?>> long getLastEventIfSubscribed(DXFeed feed, Class<E> eventTypeClass, String symbol) {
        E lastEvent = feed.getLastEventIfSubscribed(eventTypeClass, symbol);
        System.out.println("DxFeedJni::getLastEventIfSubscribed = " + lastEvent);
        if (lastEvent != null) {
            long id = nativeHandleId.incrementAndGet();
            lastingEventMap.put(id, lastEvent);
            return id;
        } else {
            return 0;
        }
    }

    private static <E extends LastingEvent<?>> long getLastEvent(DXFeed feed, long nativeHandleId) {
        System.out.println("DxFeedJni::getLastEvent");
        LastingEvent<?> lastingEvent = lastingEventMap.get(nativeHandleId);
        System.out.println("event before getLastEvent = " + lastingEvent);
        feed.getLastEvent(lastingEvent);
        System.out.println("event after getLastEvent = " + lastingEvent);
        lastingEventMap.put(nativeHandleId, lastingEvent);
        return nativeHandleId;
    }

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback, long userData);

    private static native void nOnEventListener(int size, byte[] byteData, double[] doubleData,
                                                byte[] pEventTypes, long userCallback, long userData);
}
