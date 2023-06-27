package com.dxfeed.api;

import com.dxfeed.api.osub.IndexedEventSubscriptionSymbol;
import com.dxfeed.api.osub.TimeSeriesSubscriptionSymbol;
import com.dxfeed.api.osub.WildcardSymbol;
import com.dxfeed.event.IndexedEventSource;
import com.dxfeed.event.candle.CandleSymbol;

public class DxSymbolJni {
  private static WildcardSymbol newWildCardSymbol() {
    System.out.println("DxSymbolJni::newWildCardSymbol: " + WildcardSymbol.ALL);
    return WildcardSymbol.ALL;
  }

  private static CandleSymbol newCandleSymbol(String symbol) {
    CandleSymbol candleSymbol = CandleSymbol.valueOf(symbol);
    System.out.println("DxSymbolJni::newCandleSymbol: " + candleSymbol);
    return candleSymbol;
  }

  private static <T> TimeSeriesSubscriptionSymbol<T> newTimeSeriesSubscriptionSymbol(T symbol, long fromTime) {
    TimeSeriesSubscriptionSymbol<T> tssSymbol = new TimeSeriesSubscriptionSymbol<>(symbol, fromTime);
    System.out.println("DxSymbolJni::newTimeSeriesSubscriptionSymbol: " + tssSymbol);
    return tssSymbol;
  }

  private static <T> IndexedEventSubscriptionSymbol<T> newIndexedEventSubscriptionSymbol(T symbol,
                                                                                         IndexedEventSource source) {
    IndexedEventSubscriptionSymbol<T> iesSymbol = new IndexedEventSubscriptionSymbol<>(symbol, source);
    System.out.println("DxSymbolJni::newIndexedEventSubscriptionSymbol: " + iesSymbol);
    return iesSymbol;
  }
}
