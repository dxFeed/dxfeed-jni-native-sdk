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
public enum DxfgEventClazzT {
  DXFG_EVENT_QUOTE,      // LASTING
  DXFG_EVENT_PROFILE,        // LASTING
  DXFG_EVENT_SUMMARY,        // LASTING
  DXFG_EVENT_GREEKS,         // LASTING + INDEXED -> TIME_SERIES
  DXFG_EVENT_CANDLE,         // LASTING + INDEXED -> TIME_SERIES
  DXFG_EVENT_DAILY_CANDLE,   // LASTING + INDEXED -> TIME_SERIES -> CANDLE
  DXFG_EVENT_UNDERLYING,     // LASTING + INDEXED -> TIME_SERIES
  DXFG_EVENT_THEO_PRICE,     // LASTING + INDEXED -> TIME_SERIES
  // abstract DXFG_EVENT_TRADE_BASE,     // LASTING
  DXFG_EVENT_TRADE,          // LASTING -> TRADE_BASE
  DXFG_EVENT_TRADE_ETH,      // LASTING -> TRADE_BASE
  DXFG_EVENT_CONFIGURATION,  // LASTING
  DXFG_EVENT_MESSAGE,        //
  DXFG_EVENT_TIME_AND_SALE,  // INDEXED -> TIME_SERIES
  DXFG_EVENT_ORDER_BASE,     // INDEXED
  DXFG_EVENT_ORDER,          // INDEXED -> ORDER_BASE
  DXFG_EVENT_ANALYTIC_ORDER, // INDEXED -> ORDER_BASE -> ORDER
  DXFG_EVENT_SPREAD_ORDER,   // INDEXED -> ORDER_BASE
  DXFG_EVENT_SERIES,         // INDEXED
  DXFG_EVENT_OPTION_SALE;    // INDEXED


  public byte eventOrdinal() {
    return (byte) this.ordinal();
  }

  public static byte fromClass(Class<? extends EventType<?>> clazz) {
    String canonicalName = clazz.getCanonicalName();
    if (isEquals(Quote.class, canonicalName)) {
      return DXFG_EVENT_QUOTE.eventOrdinal();
    } else if (isEquals(Profile.class, canonicalName)) {
      return DXFG_EVENT_PROFILE.eventOrdinal();
    } else if (isEquals(Summary.class, canonicalName)) {
      return DXFG_EVENT_SUMMARY.eventOrdinal();
    } else if (isEquals(Greeks.class, canonicalName)) {
      return DXFG_EVENT_GREEKS.eventOrdinal();
    } else if (isEquals(Candle.class, canonicalName)) {
      return DXFG_EVENT_CANDLE.eventOrdinal();
    } else if (isEquals(DailyCandle.class, canonicalName)) {
      return DXFG_EVENT_DAILY_CANDLE.eventOrdinal();
    } else if (isEquals(Underlying.class, canonicalName)) {
      return DXFG_EVENT_UNDERLYING.eventOrdinal();
    } else if (isEquals(TheoPrice.class, canonicalName)) {
      return DXFG_EVENT_THEO_PRICE.eventOrdinal();
    } else if (isEquals(Trade.class, canonicalName)) {
      return DXFG_EVENT_TRADE.eventOrdinal();
    } else if (isEquals(TradeETH.class, canonicalName)) {
      return DXFG_EVENT_TRADE_ETH.eventOrdinal();
    } else if (isEquals(Configuration.class, canonicalName)) {
      return DXFG_EVENT_CONFIGURATION.eventOrdinal();
    } else if (isEquals(Message.class, canonicalName)) {
      return DXFG_EVENT_MESSAGE.eventOrdinal();
    } else if (isEquals(TimeAndSale.class, canonicalName)) {
      return DXFG_EVENT_TIME_AND_SALE.eventOrdinal();
    } else if (isEquals(OrderBase.class, canonicalName)) {
      return DXFG_EVENT_ORDER_BASE.eventOrdinal();
    } else if (isEquals(Order.class, canonicalName)) {
      return DXFG_EVENT_ORDER.eventOrdinal();
    } else if (isEquals(AnalyticOrder.class, canonicalName)) {
      return DXFG_EVENT_ANALYTIC_ORDER.eventOrdinal();
    } else if (isEquals(SpreadOrder.class, canonicalName)) {
      return DXFG_EVENT_SPREAD_ORDER.eventOrdinal();
    } else if (isEquals(Series.class, canonicalName)) {
      return DXFG_EVENT_SERIES.eventOrdinal();
    } else if (isEquals(OptionSale.class, canonicalName)) {
      return DXFG_EVENT_OPTION_SALE.eventOrdinal();
    } else {
      throw new IllegalStateException("Can't find DxfgEventClazzT for " + canonicalName);
    }
  }

  private static boolean isEquals(Class<? extends EventType<?>> classType, String canonicalName) {
    return canonicalName.equals(classType.getCanonicalName());
  }
}
