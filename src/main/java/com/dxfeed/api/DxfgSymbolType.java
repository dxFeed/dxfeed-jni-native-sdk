package com.dxfeed.api;

// https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/java/com/dxfeed/sdk/symbol/DxfgSymbolType.java#L10
public enum DxfgSymbolType {
    STRING,
    CANDLE,
    WILDCARD,
    INDEXED_EVENT_SUBSCRIPTION,
    TIME_SERIES_SUBSCRIPTION
}
