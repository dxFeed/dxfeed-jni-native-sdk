package com.dxfeed.api;

import com.dxfeed.event.EventType;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;

class JNIDXFeedEventListener {
    private static final ConcurrentHashMap<Long, DXFeedEventListener<EventType<?>>> eventListenerMap = new ConcurrentHashMap<>();
    private static final AtomicLong nativeHandle = new AtomicLong();

    // callback from native
    private static long newEventListener(long userCallback, long userData) {
        long id = nativeHandle.incrementAndGet();
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
            System.out.println("addEventListener, nativeHandle = " + nativeHandleId);
            sub.addEventListener(eventListener);
        }
    }

    private static void removeEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
        DXFeedEventListener<EventType<?>> eventListener = eventListenerMap.remove(nativeHandleId);
        if (eventListener != null) {
            System.out.println("removeEventListener, nativeHandle = " + nativeHandleId);
            sub.removeEventListener(eventListener);
        }
    }

    private static native void nOnEventListener(int size, byte[] byteData, double[] doubleData,
                                                byte[] pEventTypes, long userCallback, long userData);
}
