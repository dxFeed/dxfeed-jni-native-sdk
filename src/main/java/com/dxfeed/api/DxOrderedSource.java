package com.dxfeed.api;

import com.dxfeed.event.IndexedEventSource;
import com.dxfeed.event.market.OrderSource;

import java.util.concurrent.ConcurrentHashMap;

public class DxOrderedSource {
    private static final ConcurrentHashMap<Long, IndexedEventSource> indexedEventSource = new ConcurrentHashMap<>();

    private static void newOrderedSource(String name, long[/* 3 */] data) {
        long nativeHandleId = DxFeedJni.nextHandleId();
        IndexedEventSource source = (name == null) ? IndexedEventSource.DEFAULT : OrderSource.valueOf(name);
        indexedEventSource.put(nativeHandleId, source);
        data[0] = (source instanceof OrderSource)
                ? DxfgIndexedEventSourceType.ORDER_SOURCE.ordinal()
                : DxfgIndexedEventSourceType.INDEXED_EVENT_SOURCE.ordinal();
        data[1] = source.id();
        data[2] = nativeHandleId;
        System.out.println("DxOrderedSource::newOrderedSource, source = " + source +
                ", nativeHandleId = " + nativeHandleId);
    }

    private static void releaseOrderedSource(long nativeHandleId) {
        IndexedEventSource remove = indexedEventSource.remove(nativeHandleId);
        if (remove != null) {
            System.out.println("DxOrderedSource::releaseOrderedSource, source = " + remove);
        }
    }
}