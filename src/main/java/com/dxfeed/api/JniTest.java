package com.dxfeed.api;

import com.dxfeed.event.EventType;

import java.beans.PropertyChangeListener;
import java.util.concurrent.ConcurrentHashMap;

public class JniTest {
    private static final ConcurrentHashMap<Long, PropertyChangeListener> changeListenerMap = new ConcurrentHashMap<>();

    static {
        String property = System.getProperty("com.devexperts.qd.impl.matrix.Agent.MaxBufferSize");
        System.out.println("[JniTest]: After loading class " + JniTest.class.getName() +
                ", com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = " + property);
    }

    // callback from native
    private static void addEventListener(DXFeedSubscription<EventType<?>> sub, long userCallback) {
        System.out.println("addEventListener, dxFeedSub = " + sub + "; userCallback = " + userCallback);

        sub.addEventListener(eventList -> {
            EventsNative nativeTS = new EventsNative(eventList);
            nOnQuoteEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
                    userCallback);
            nativeTS.clear();
            nativeTS = null;
        });
    }

    private static void addStateChangeEventListener(DXEndpoint endpoint, long userCallback) {
        System.out.println("addStateChangeEventListener, dxEndpoint = " + endpoint + "; userCallback = " + userCallback);
        PropertyChangeListener propertyChangeListener = changeEvent -> nOnStateChangeListener(
                ((DXEndpoint.State) changeEvent.getOldValue()).ordinal(),
                ((DXEndpoint.State) changeEvent.getNewValue()).ordinal(),
                userCallback
        );
        endpoint.addStateChangeListener(propertyChangeListener);
        changeListenerMap.put(userCallback, propertyChangeListener);
    }

    private static void removeStateChangeEventListener(DXEndpoint endpoint, long userCallback) {
        System.out.println("removeStateChangeEventListener, dxEndpoint = " + endpoint + "; userCallback = " + userCallback);
        PropertyChangeListener propertyChangeListener = changeListenerMap.get(userCallback);
        endpoint.removeStateChangeListener(propertyChangeListener);
    }


    private static native void nOnQuoteEventListener(int size, byte[] byteData, double[] doubleData,
                                                     byte[] pEventTypes, long userCallback);

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback);
}
