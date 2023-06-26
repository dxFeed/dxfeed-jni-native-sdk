package com.dxfeed.api;

import com.dxfeed.event.LastingEvent;
import com.dxfeed.event.market.Quote;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;

public class DxFeedJni {
    static {
        String property = System.getProperty("com.devexperts.qd.impl.matrix.Agent.MaxBufferSize");
        System.out.println("[DxFeedJni]: After loading class " + DxFeedJni.class.getName() +
                ", com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = " + property);
    }

    private static class NativeEventData {
        private final byte[] byteData;
        private final double[] doubleData;

        public NativeEventData(NativeEventsList nativeEventsList) {
            byteData = nativeEventsList.byteData();
            doubleData = nativeEventsList.doubleData();
        }
    }

    public static final ConcurrentHashMap<Long, Object> nativeObjectsMap = new ConcurrentHashMap<>();
    private static final AtomicLong nativeHandleId = new AtomicLong();

    public static long nextHandleId() {
        return nativeHandleId.incrementAndGet();
    }

    private static <E extends LastingEvent<?>> NativeEventData getLastEventIfSubscribed(DXFeed feed,
                                                                                        Class<E> eventTypeClass,
                                                                                        Object symbol)
    {
        E lastEvent = feed.getLastEventIfSubscribed(eventTypeClass, symbol);
        System.out.println("DxFeedJni::getLastEventIfSubscribed = " + lastEvent);
        return (lastEvent != null) ? new NativeEventData(new NativeEventsList(List.of(lastEvent))) : null;
    }

    private static <E extends LastingEvent<?>> NativeEventData getLastEvent(DXFeed feed, Class<E> eventTypeClass,
                                                                            String eventName)
    {
        E event = createEventByClass(eventTypeClass, eventName);
        System.out.println("event before getLastEvent = " + event);
        feed.getLastEvent(event);
        System.out.println("event after getLastEvent = " + event);
        if (event instanceof Quote) {
            return new NativeEventData(new NativeEventsList(List.of(event)));
        }
        return null;
    }

    private static <E extends LastingEvent<?>> E createEventByClass(Class<E> eventTypeClass, String eventName) {
        try {
            Constructor<E> constructor = eventTypeClass.getConstructor(String.class);
            System.out.println("constructor = " + constructor);
            E event = constructor.newInstance(eventName);
            System.out.println("event = " + event);
            return event;
        } catch (NoSuchMethodException | InstantiationException | IllegalAccessException | InvocationTargetException e) {
            throw new RuntimeException(e);
        }
    }
}
