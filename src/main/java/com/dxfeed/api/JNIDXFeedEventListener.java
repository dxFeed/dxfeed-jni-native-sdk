package com.dxfeed.api;

import com.dxfeed.event.EventType;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

class JNIDXFeedEventListener implements DXFeedEventListener<EventType<?>> {
    private static final ConcurrentHashMap<Long, JNIDXFeedEventListener> eventListenerMap = new ConcurrentHashMap<>();
    private long nativeHandle;

    JNIDXFeedEventListener(long handle) {
        nativeHandle = handle;
        System.out.println("init JNIDXFeedEventListener with nativeHandle, handle = " + nativeHandle);
    }

    @Override
    public void eventsReceived(List<EventType<?>> eventList) {
        EventsNative nativeTS = new EventsNative(eventList);
        nOnEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
                nativeHandle);
        nativeTS.clear();
        nativeTS = null;
    }

    public void close() {
        nClose(nativeHandle);
        System.out.println("release JNIDXFeedEventListener with nativeHandle, handle = " + nativeHandle);
        nativeHandle = 0;
    }

    // callback from native
    private static JNIDXFeedEventListener newEventListener(long nativeHandle) {
        return new JNIDXFeedEventListener(nativeHandle);
    }

    private static void addEventListener(DXFeedSubscription<EventType<?>> sub, JNIDXFeedEventListener listener) {
        sub.addEventListener(listener);
        eventListenerMap.put(listener.nativeHandle, listener);
    }

    private static void removeEventListener(DXFeedSubscription<EventType<?>> sub, JNIDXFeedEventListener listener) {
        JNIDXFeedEventListener eventListener = eventListenerMap.remove(listener.nativeHandle);
        if (eventListener != null) {
            sub.removeEventListener(eventListener);
            eventListener.close();
        }
    }

    private static native void nOnEventListener(int size, byte[] byteData, double[] doubleData,
                                                byte[] pEventTypes, long userCallback);
    private static native void nClose(long nativeHandle);
}
