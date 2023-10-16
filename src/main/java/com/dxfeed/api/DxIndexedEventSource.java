package com.dxfeed.api;

import com.dxfeed.event.IndexedEventSource;
import com.dxfeed.event.market.OrderSource;

public class DxIndexedEventSource {
  private static IndexedEventSource newOrderSourceByName(String name, long[/* 2 */] data) {
    IndexedEventSource source = (name == null) ? IndexedEventSource.DEFAULT : OrderSource.valueOf(name);
    data[0] = (source instanceof OrderSource)
        ? DxfgIndexedEventSourceType.ORDER_SOURCE.ordinal()
        : DxfgIndexedEventSourceType.INDEXED_EVENT_SOURCE.ordinal();
    data[1] = source.id();
    return source;
  }

  private static OrderSource newOrderSourceById(int sourceId, long[/* 2 */] data) {
    OrderSource orderSource = OrderSource.valueOf(sourceId);
    data[0] = DxfgIndexedEventSourceType.ORDER_SOURCE.ordinal();
    data[1] = orderSource.id();
    return orderSource;
  }
}