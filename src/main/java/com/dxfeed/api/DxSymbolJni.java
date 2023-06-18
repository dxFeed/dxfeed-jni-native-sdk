package com.dxfeed.api;

import com.dxfeed.api.osub.WildcardSymbol;
import com.dxfeed.event.candle.CandleSymbol;

import java.util.concurrent.ConcurrentHashMap;

public class DxSymbolJni {
    private static final ConcurrentHashMap<Long, Object> symbolMap = new ConcurrentHashMap<>();

    private static long newSymbol(String symbol, int symbolType) {
        boolean isTimeSeries = symbolType == DxfgSymbolType.TIME_SERIES_SUBSCRIPTION.ordinal();
        boolean isIndexedEvent = symbolType == DxfgSymbolType.INDEXED_EVENT_SUBSCRIPTION.ordinal();
        if (isTimeSeries || isIndexedEvent) {
            throw new IllegalStateException();
        }
        long id = DxFeedJni.nextHandleId();
        System.out.println("DxSymbolJni::newSymbol, nativeHandleId = " + id);
        if (symbolType == DxfgSymbolType.STRING.ordinal()) {
            symbolMap.put(id, symbol);
        } else if (symbolType == DxfgSymbolType.CANDLE.ordinal()) {
            CandleSymbol candleSymbol = CandleSymbol.valueOf(symbol);
            symbolMap.put(id, candleSymbol);
        } else if (symbolType == DxfgSymbolType.WILDCARD.ordinal()) {
            symbolMap.put(id, WildcardSymbol.ALL);
        } else {
            throw new IllegalStateException();
        }
        return id;
    }

    private static void releaseSymbol(long nativeHandlerId) {
        Object remove = symbolMap.remove(nativeHandlerId);
        if (remove != null) {
            System.out.println("DxSymbolJni::releaseSymbol, nativeHandleId = " + nativeHandlerId);
        }
    }
}
