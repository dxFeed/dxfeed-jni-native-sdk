package com.dxfeed.api;

import com.dxfeed.event.market.TimeAndSale;

public class JniTest {
    static {
        String property = System.getProperty("com.devexperts.qd.impl.matrix.Agent.MaxBufferSize");
        System.out.println("[JniTest]: After loading class " + JniTest.class.getName() +
                ", com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = " + property);
    }

    // callback from native
    private static void addEventListener(DXFeedSubscription<TimeAndSale> sub, long userCallback) {
        System.out.println("addEventListener, dxFeedSub = " + sub + "; userCallback = " + userCallback);

        sub.addEventListener(eventList -> {
            TimeAndSalesNative nativeTS = new TimeAndSalesNative(eventList);
            nOnQuoteEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), userCallback);
        });
    }

    private static native void nOnQuoteEventListener(int size, byte[] byteData, double[] doubleData, long userCallback);
}
