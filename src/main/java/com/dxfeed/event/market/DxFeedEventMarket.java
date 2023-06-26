package com.dxfeed.event.market;

public class DxFeedEventMarket {
    public static class TimeAndSalePackagePrivate {
        public static int getFlags(TimeAndSale event) {
            return event.getFlags(); // package private method
        }
    }

    public static class TradeBasePackagePrivate {
        public static int getFlags(TradeBase event) {
            return event.getFlags(); // package private method
        }
    }

    public static class ProfilePackagePrivate {
        public static int getFlags(Profile event) {
            return event.getFlags(); // package private method
        }
    }

    public static class QuotePackagePrivate {
        public static int getTimeMillisSequence(Quote event) {
            return event.getTimeMillisSequence(); // package private method
        }
    }
}
