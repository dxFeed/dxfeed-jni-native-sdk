package com.dxfeed.api;

import com.dxfeed.event.EventType;
import com.dxfeed.event.candle.Candle;
import com.dxfeed.event.candle.DailyCandle;
import com.dxfeed.event.market.*;
import com.dxfeed.event.misc.Configuration;
import com.dxfeed.event.misc.Message;
import com.dxfeed.event.option.Greeks;
import com.dxfeed.event.option.Series;
import com.dxfeed.event.option.TheoPrice;
import com.dxfeed.event.option.Underlying;

// https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#LL75C30-L75C30
public final class DxfgEventClazzT {
  public static final byte DXFG_EVENT_QUOTE = 0;      // LASTING
  public static final byte DXFG_EVENT_PROFILE = 1;        // LASTING
  public static final byte DXFG_EVENT_SUMMARY = 2;        // LASTING
  public static final byte DXFG_EVENT_GREEKS = 3;         // LASTING + INDEXED -> TIME_SERIES
  public static final byte DXFG_EVENT_CANDLE = 4;         // LASTING + INDEXED -> TIME_SERIES
  public static final byte DXFG_EVENT_DAILY_CANDLE = 5;   // LASTING + INDEXED -> TIME_SERIES -> CANDLE
  public static final byte DXFG_EVENT_UNDERLYING = 6;     // LASTING + INDEXED -> TIME_SERIES
  public static final byte DXFG_EVENT_THEO_PRICE = 7;     // LASTING + INDEXED -> TIME_SERIES
  // abstract DXFG_EVENT_TRADE_BASE,     // LASTING
  public static final byte DXFG_EVENT_TRADE = 8;          // LASTING -> TRADE_BASE
  public static final byte DXFG_EVENT_TRADE_ETH = 9;      // LASTING -> TRADE_BASE
  public static final byte DXFG_EVENT_CONFIGURATION = 10;  // LASTING
  public static final byte DXFG_EVENT_MESSAGE = 11;        //
  public static final byte DXFG_EVENT_TIME_AND_SALE = 12;  // INDEXED -> TIME_SERIES
  public static final byte DXFG_EVENT_ORDER_BASE = 13;     // INDEXED
  public static final byte DXFG_EVENT_ORDER = 14;          // INDEXED -> ORDER_BASE
  public static final byte DXFG_EVENT_ANALYTIC_ORDER = 15; // INDEXED -> ORDER_BASE -> ORDER
  public static final byte DXFG_EVENT_SPREAD_ORDER = 16;   // INDEXED -> ORDER_BASE
  public static final byte DXFG_EVENT_SERIES = 17;         // INDEXED
  public static final byte DXFG_EVENT_OPTION_SALE = 18;    // INDEXED

  public static byte fromClass(Class<? extends EventType<?>> clazz) {
    String canonicalName = clazz.getCanonicalName();
    if (isEquals(Quote.class, canonicalName)) {
      return DXFG_EVENT_QUOTE;
    } else if (isEquals(Profile.class, canonicalName)) {
      return DXFG_EVENT_PROFILE;
    } else if (isEquals(Summary.class, canonicalName)) {
      return DXFG_EVENT_SUMMARY;
    } else if (isEquals(Greeks.class, canonicalName)) {
      return DXFG_EVENT_GREEKS;
    } else if (isEquals(Candle.class, canonicalName)) {
      return DXFG_EVENT_CANDLE;
    } else if (isEquals(DailyCandle.class, canonicalName)) {
      return DXFG_EVENT_DAILY_CANDLE;
    } else if (isEquals(Underlying.class, canonicalName)) {
      return DXFG_EVENT_UNDERLYING;
    } else if (isEquals(TheoPrice.class, canonicalName)) {
      return DXFG_EVENT_THEO_PRICE;
    } else if (isEquals(Trade.class, canonicalName)) {
      return DXFG_EVENT_TRADE;
    } else if (isEquals(TradeETH.class, canonicalName)) {
      return DXFG_EVENT_TRADE_ETH;
    } else if (isEquals(Configuration.class, canonicalName)) {
      return DXFG_EVENT_CONFIGURATION;
    } else if (isEquals(Message.class, canonicalName)) {
      return DXFG_EVENT_MESSAGE;
    } else if (isEquals(TimeAndSale.class, canonicalName)) {
      return DXFG_EVENT_TIME_AND_SALE;
    } else if (isEquals(OrderBase.class, canonicalName)) {
      return DXFG_EVENT_ORDER_BASE;
    } else if (isEquals(Order.class, canonicalName)) {
      return DXFG_EVENT_ORDER;
    } else if (isEquals(AnalyticOrder.class, canonicalName)) {
      return DXFG_EVENT_ANALYTIC_ORDER;
    } else if (isEquals(SpreadOrder.class, canonicalName)) {
      return DXFG_EVENT_SPREAD_ORDER;
    } else if (isEquals(Series.class, canonicalName)) {
      return DXFG_EVENT_SERIES;
    } else if (isEquals(OptionSale.class, canonicalName)) {
      return DXFG_EVENT_OPTION_SALE;
    } else {
      throw new IllegalStateException("Can't find DxfgEventClazzT for " + canonicalName);
    }
  }

  private static boolean isEquals(Class<? extends EventType<?>> classType, String canonicalName) {
    return canonicalName.equals(classType.getCanonicalName());
  }
}
