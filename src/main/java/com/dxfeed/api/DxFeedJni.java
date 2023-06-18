package com.dxfeed.api;

import com.dxfeed.event.LastingEvent;

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

    private static final ConcurrentHashMap<Long, LastingEvent<?>> lastingEventMap = new ConcurrentHashMap<>();
    private static final AtomicLong nativeHandleId = new AtomicLong();

    public static long nextHandleId() {
        return nativeHandleId.incrementAndGet();
    }

    private static <E extends LastingEvent<?>> long newEvent(Class<E> eventTypeClass, String symbol) {
        long id = nativeHandleId.incrementAndGet();
        System.out.println("DxFeedJni::newEvent, nativeHandle = " + id);
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

    private static void releaseEvent(long nativeHandlerId) {
        LastingEvent<?> event = lastingEventMap.remove(nativeHandlerId);
        if (event != null) {
            System.out.println("DxFeedJni::releaseEvent, nativeHandle = " + nativeHandlerId);
        }
    }

    private static <E extends LastingEvent<?>> long getLastEventIfSubscribed(DXFeed feed, Class<E> eventTypeClass,
                                                                             String symbol)
    {
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
        System.out.println("DxFeedJni::getLastEvent, nativeHandleId = " + nativeHandleId);
        LastingEvent<?> lastingEvent = lastingEventMap.get(nativeHandleId);
        System.out.println("event before getLastEvent = " + lastingEvent);
        feed.getLastEvent(lastingEvent);
        System.out.println("event after getLastEvent = " + lastingEvent);
        lastingEventMap.put(nativeHandleId, lastingEvent);
        return nativeHandleId;
    }
}
