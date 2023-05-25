package com.dxfeed.event.market;

public class DxFeedEventMarket {
    public static class TimeAndSalePackagePrivate {
        public static int getFlags(TimeAndSale event) {
            return event.getFlags(); // package private method
        }
    }
}
