package com.dxfeed.api;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.concurrent.ConcurrentHashMap;

class JNIPropertyChangeListener implements PropertyChangeListener {
    private static final ConcurrentHashMap<Long, JNIPropertyChangeListener> changeListenerMap = new ConcurrentHashMap<>();
    private long nativeHandle;

    JNIPropertyChangeListener(long handle) {
        nativeHandle = handle;
        System.out.println("init JNIPropertyChangeListener with nativeHandle, handle = " + nativeHandle);
    }

    @Override
    public void propertyChange(PropertyChangeEvent event) {
        nOnStateChangeListener(
                ((DXEndpoint.State) event.getOldValue()).ordinal(),
                ((DXEndpoint.State) event.getNewValue()).ordinal(),
                nativeHandle);
    }

    public void close() {
        nClose(nativeHandle);
        System.out.println("release JNIPropertyChangeListener with nativeHandle, handle = " + nativeHandle);
        nativeHandle = 0;
    }

    // callbacks from native
    private static JNIPropertyChangeListener newStateChangeEventListener(long nativeHandle) {
        return new JNIPropertyChangeListener(nativeHandle);
    }

    private static void addStateChangeEventListener(DXEndpoint endpoint, JNIPropertyChangeListener listener) {
        System.out.println("addStateChangeEventListener, listener.nativeHandle = " + listener.nativeHandle);
        endpoint.addStateChangeListener(listener);
        changeListenerMap.put(listener.nativeHandle, listener);
    }

    private static void removeStateChangeEventListener(DXEndpoint endpoint, JNIPropertyChangeListener listener) {
        JNIPropertyChangeListener stateChangeListener = changeListenerMap.remove(listener.nativeHandle);
        if (stateChangeListener != null) {
            System.out.println("removeStateChangeEventListener, listener.nativeHandle = " + listener.nativeHandle);
            endpoint.removeStateChangeListener(listener);
            listener.close();
        }
    }

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback);
    private static native void nClose(long nativeHandle);

}
