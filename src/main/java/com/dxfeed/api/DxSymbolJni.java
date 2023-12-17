// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api;

import com.dxfeed.api.osub.IndexedEventSubscriptionSymbol;
import com.dxfeed.api.osub.TimeSeriesSubscriptionSymbol;
import com.dxfeed.api.osub.WildcardSymbol;
import com.dxfeed.event.IndexedEventSource;
import com.dxfeed.event.candle.CandleSymbol;

public class DxSymbolJni {
  private static WildcardSymbol newWildCardSymbol() {
    return WildcardSymbol.ALL;
  }

  private static CandleSymbol newCandleSymbol(String symbol) {
    return CandleSymbol.valueOf(symbol);
  }

  private static <T> TimeSeriesSubscriptionSymbol<T> newTimeSeriesSubscriptionSymbol(T symbol, long fromTime) {
    return new TimeSeriesSubscriptionSymbol<>(symbol, fromTime);
  }

  private static <T> IndexedEventSubscriptionSymbol<T> newIndexedEventSubscriptionSymbol(T symbol,
                                                                                         IndexedEventSource source) {
    return new IndexedEventSubscriptionSymbol<>(symbol, source);
  }
}
