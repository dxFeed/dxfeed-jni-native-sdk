package com.dxfeed.event.market;

public class DxFeedEventMarketPackagePrivate {
  public static int getFlags(TimeAndSale event) {
    return event.getFlags();
  }

  public static void setFlags(TimeAndSale event, int value) {
    event.setFlags(value);
  }

  public static int getFlags(TradeBase event) {
    return event.getFlags();
  }

  public static void setFlags(TradeBase event, int value) {
    event.setFlags(value);
  }

  public static int getFlags(Profile event) {
    return event.getFlags();
  }

  public static void setFlags(Profile event, int value) {
    event.setFlags(value);
  }

  public static int getFlags(Summary event) {
    return event.getFlags();
  }

  public static void setFlags(Summary event, int value) {
    event.setFlags(value);
  }

  public static int getTimeMillisSequence(Quote event) {
    return event.getTimeMillisSequence();
  }

  public static void setTimeMillisSequence(Quote event, int value) {
    event.setTimeMillisSequence(value);
  }

  public static int getFlags(OptionSale event) {
    return event.getFlags();
  }

  public static void setFlags(OptionSale event, int value) {
    event.setFlags(value);
  }

  public static OrderBase createOrderBase() {
    return new OrderBase();
  }

  public static int getFlags(OrderBase event) {
    return event.getFlags();
  }

  public static void setFlags(OrderBase event, int value) {
    event.setFlags(value);
  }

  public static int getIcebergFlags(AnalyticOrder event) {
    return event.getIcebergFlags();
  }

  public static void setIcebergFlags(AnalyticOrder event, int value) {
    event.setIcebergFlags(value);
  }
}
