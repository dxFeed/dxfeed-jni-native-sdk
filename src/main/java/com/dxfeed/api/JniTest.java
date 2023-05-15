package com.dxfeed.api;

import com.dxfeed.event.EventType;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.List;

public class JniTest {
    static {
        String property = System.getProperty("com.devexperts.qd.impl.matrix.Agent.MaxBufferSize");
        System.out.println("[JniTest]: After loading class " + JniTest.class.getName() +
                ", com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = " + property);
    }

    private static class JNIDXFeedEventListener implements DXFeedEventListener<EventType<?>> {
        private final long userCallback;

        JNIDXFeedEventListener(long userCallback) {
            this.userCallback = userCallback;
        }
        @Override
        public void eventsReceived(List<EventType<?>> eventList) {
//            System.out.println("call DXFeedEventListener with userCallback = " + userCallback);
            EventsNative nativeTS = new EventsNative(eventList);
            nOnQuoteEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
                    userCallback);
            nativeTS.clear();
            nativeTS = null;
        }
    }

    private static class JNIPropertyChangeListener implements PropertyChangeListener {
        private final long userCallback;

        JNIPropertyChangeListener(long userCallback) {
            this.userCallback = userCallback;
        }

        @Override
        public void propertyChange(PropertyChangeEvent event) {
//            System.out.println("call PropertyChangeEvent with userCallback = " + userCallback);
            nOnStateChangeListener(
                    ((DXEndpoint.State) event.getOldValue()).ordinal(),
                    ((DXEndpoint.State) event.getNewValue()).ordinal(),
                    userCallback);
        }
    }

    // callback from native
    private static DXFeedEventListener<EventType<?>> newEventListener(long userCallback) {
        return new JNIDXFeedEventListener(userCallback);
    }

    private static PropertyChangeListener newStateChangeEventListener(long userCallback) {
        return new JNIPropertyChangeListener(userCallback);
    }

    private static native void nOnQuoteEventListener(int size, byte[] byteData, double[] doubleData,
                                                     byte[] pEventTypes, long userCallback);

    private static native void nOnStateChangeListener(int oldState, int newState, long userCallback);
}
