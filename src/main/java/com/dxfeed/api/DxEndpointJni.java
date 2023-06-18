package com.dxfeed.api;

import java.beans.PropertyChangeListener;
import java.util.concurrent.ConcurrentHashMap;

public class DxEndpointJni {
    private static final ConcurrentHashMap<Long, PropertyChangeListener> changeListenerMap = new ConcurrentHashMap<>();

    // callbacks from native
    private static long newStateChangeEventListener(long userCallback, long userData) {
        long id = DxFeedJni.nextHandleId();
        System.out.println("DxEndpointJni::newStateChangeEventListener, nativeHandle = " + id);
        changeListenerMap.put(id, event -> nOnStateChangeListener(
                ((DXEndpoint.State) event.getOldValue()).ordinal(),
                ((DXEndpoint.State) event.getNewValue()).ordinal(),
                userCallback, userData));
        return id;
    }

    private static void addStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
        PropertyChangeListener propertyChangeListener = changeListenerMap.get(nativeHandleId);
        if (propertyChangeListener != null) {
            System.out.println("DxEndpointJni::addStateChangeEventListener, nativeHandle = " + nativeHandleId);
            endpoint.addStateChangeListener(propertyChangeListener);
        }
    }

    private static void removeStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
        PropertyChangeListener stateChangeListener = changeListenerMap.remove(nativeHandleId);
        if (stateChangeListener != null) {
            System.out.println("DxEndpointJni::removeStateChangeEventListener, nativeHandle = " + nativeHandleId);
            endpoint.removeStateChangeListener(stateChangeListener);
        }
    }

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback, long userData);
}
