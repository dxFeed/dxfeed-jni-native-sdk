package com.dxfeed.event.market;

public class DxFeedEventMarketPackagePrivate {
  public static int getFlags(TimeAndSale event) {
    return event.getFlags();
  }

  public static int getFlags(TradeBase event) {
    return event.getFlags();
  }

  public static int getFlags(Profile event) {
    return event.getFlags();
  }

  public static int getFlags(Summary event) {
    return event.getFlags();
  }

  public static int getTimeMillisSequence(Quote event) {
    return event.getTimeMillisSequence();
  }

  public static int getFlags(OptionSale event) {
    return event.getFlags();
  }

  public static int getFlags(OrderBase event) {
    return event.getFlags();
  }

  public static int getIcebergFlags(AnalyticOrder event) {
    return event.getIcebergFlags();
  }
}
