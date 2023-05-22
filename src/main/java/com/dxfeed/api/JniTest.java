package com.dxfeed.api;

import com.dxfeed.event.EventType;

import java.beans.PropertyChangeListener;

public class JniTest {
    static {
        String property = System.getProperty("com.devexperts.qd.impl.matrix.Agent.MaxBufferSize");
        System.out.println("[JniTest]: After loading class " + JniTest.class.getName() +
                ", com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = " + property);
    }

    // callback from native
    private static DXFeedEventListener<EventType<?>> newEventListener(long userCallback) {
        System.out.println("newEventListener, with  userCallback = " + userCallback);
        return eventList -> {
            EventsNative nativeTS = new EventsNative(eventList);
            nOnQuoteEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
                    userCallback);
            nativeTS.clear();
            nativeTS = null;
        };
    }

    private static PropertyChangeListener newStateChangeEventListener(long userCallback) {
        System.out.println("newStateChangeEventListener, with  userCallback = " + userCallback);
        return evt -> nOnStateChangeListener(
                ((DXEndpoint.State) evt.getOldValue()).ordinal(),
                ((DXEndpoint.State) evt.getNewValue()).ordinal(),
                userCallback
        );
    }

    private static native void nOnQuoteEventListener(int size, byte[] byteData, double[] doubleData,
                                                     byte[] pEventTypes, long userCallback);

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback);
}
