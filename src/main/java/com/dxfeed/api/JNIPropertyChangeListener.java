package com.dxfeed.api;

import java.beans.PropertyChangeListener;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;

class JNIPropertyChangeListener {
    private static final ConcurrentHashMap<Long, PropertyChangeListener> changeListenerMap = new ConcurrentHashMap<>();
    private static final AtomicLong nativeHandle = new AtomicLong();

    // callbacks from native
    private static long newStateChangeEventListener(long userCallback, long userData) {
        long id = nativeHandle.incrementAndGet();
        changeListenerMap.put(id, event -> nOnStateChangeListener(
                ((DXEndpoint.State) event.getOldValue()).ordinal(),
                ((DXEndpoint.State) event.getNewValue()).ordinal(),
                userCallback, userData));
        return id;
    }

    private static void addStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
        PropertyChangeListener propertyChangeListener = changeListenerMap.get(nativeHandleId);
        if (propertyChangeListener != null) {
            System.out.println("addStateChangeEventListener, nativeHandle = " + nativeHandleId);
            endpoint.addStateChangeListener(propertyChangeListener);
        }
    }

    private static void removeStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
        PropertyChangeListener stateChangeListener = changeListenerMap.remove(nativeHandleId);
        if (stateChangeListener != null) {
            System.out.println("removeStateChangeEventListener, nativeHandle = " + nativeHandleId);
            endpoint.removeStateChangeListener(stateChangeListener);
        }
    }

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback, long userData);
}
