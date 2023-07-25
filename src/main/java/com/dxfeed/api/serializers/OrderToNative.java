package com.dxfeed.api.serializers;

import com.dxfeed.api.DxfgEventClazzT;
import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.*;

public class OrderToNative {
  public static byte convert(OrderBase event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    if (event instanceof AnalyticOrder) {
      return convertAnalyticsOrder((AnalyticOrder) event, pBytes, pDoubles, chunkIdx);
    } else if (event instanceof Order) {
      return convertOrder((Order)event, pBytes, pDoubles, chunkIdx);
    } else if (event instanceof SpreadOrder) {
      return convertSpreadOrder((SpreadOrder) event, pBytes, pDoubles, chunkIdx);
    } else {
      return convertOrderBase(event, pBytes, pDoubles, chunkIdx);
    }
  }

  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L384
   *
   * typedef struct dxfg_order_base_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_indexed_event_t indexed_event;
   *    int32_t event_flags;
   *    int64_t index;
   *    int64_t time_sequence;
   *    int32_t time_nano_part;
   *    int64_t action_time;
   *    int64_t order_id;
   *    int64_t aux_order_id;
   *    double price;
   *    double size;
   *    double executed_size;
   *    int64_t count;
   *    int32_t flags;
   *    int64_t trade_id;
   *    double trade_price;
   *    double trade_size;
   * } dxfg_order_base_t;
   */

  private static byte convertOrderBase(OrderBase event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                  // 8
    int eventFlags = event.getEventFlags();                                 // 4
    long index = event.getIndex();                                          // 8
    long timeSequence = event.getTimeSequence();                            // 8
    int timeNanoPart = event.getTimeNanoPart();                             // 4
    long actionTime = event.getActionTime();                                // 8
    long orderId = event.getOrderId();                                      // 8
    long auxOrderId = event.getAuxOrderId();                                // 8
    long count = event.getCount();                                          // 8
    int flags = DxFeedEventMarketPackagePrivate.getFlags(event);            // 4
    long tradeId = event.getTradeId();                                      // 8

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8) + (8) + (4) + (8) + (8) + (8) + (8) + (4) + (8);

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);            // 2 + eventSymbolLength
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8
    pBytes.writeLong(timeSequence);             // 8
    pBytes.writeInt(timeNanoPart);              // 4
    pBytes.writeLong(actionTime);               // 8
    pBytes.writeLong(orderId);                  // 8
    pBytes.writeLong(auxOrderId);               // 8
    pBytes.writeLong(count);                    // 8
    pBytes.writeInt(flags);                     // 4
    pBytes.writeLong(tradeId);                  // 8

    // DOUBLE DATA
    double price = event.getPrice();                // 1
    double size = event.getSize();                  // 1
    double executedSize = event.getExecutedSize();  // 1
    double tradePrice = event.getTradePrice();      // 1
    double tradeSize = event.getTradeSize();        // 1

    // DOUBLE DATA
    int doublesCount = 5;
    pDoubles.addChunk(chunkIdx, doublesCount);
    pDoubles.write(price);
    pDoubles.write(size);
    pDoubles.write(executedSize);
    pDoubles.write(tradePrice);
    pDoubles.write(tradeSize);
    return DxfgEventClazzT.DXFG_EVENT_ORDER_BASE.eventOrdinal();
  }

  /**
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L411
   * <p>
   * typedef struct dxfg_order_t {
   * dxfg_order_base_t order_base;
   * const char* market_maker;
   * } dxfg_order_t;
   *
   * @return
   */

  private static byte convertOrder(Order event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                  // 8
    int eventFlags = event.getEventFlags();                                 // 4
    long index = event.getIndex();                                          // 8
    long timeSequence = event.getTimeSequence();                            // 8
    int timeNanoPart = event.getTimeNanoPart();                             // 4
    long actionTime = event.getActionTime();                                // 8
    long orderId = event.getOrderId();                                      // 8
    long auxOrderId = event.getAuxOrderId();                                // 8
    long count = event.getCount();                                          // 8
    int flags = DxFeedEventMarketPackagePrivate.getFlags(event);            // 4
    long tradeId = event.getTradeId();                                      // 8
    CString marketMaker = new CString(event.getMarketMaker());

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8) + (8) + (4) + (8) + (8) + (8) + (8) + (4) + (8)
        + marketMaker.totalAllocatedBytes;

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8
    pBytes.writeLong(timeSequence);             // 8
    pBytes.writeInt(timeNanoPart);              // 4
    pBytes.writeLong(actionTime);               // 8
    pBytes.writeLong(orderId);                  // 8
    pBytes.writeLong(auxOrderId);               // 8
    pBytes.writeLong(count);                    // 8
    pBytes.writeInt(flags);                     // 4
    pBytes.writeLong(tradeId);                  // 8
    pBytes.writeString(marketMaker);

    // DOUBLE DATA
    double price = event.getPrice();                // 1
    double size = event.getSize();                  // 1
    double executedSize = event.getExecutedSize();  // 1
    double tradePrice = event.getTradePrice();      // 1
    double tradeSize = event.getTradeSize();        // 1

    // DOUBLE DATA
    int doublesCount = 5;
    pDoubles.addChunk(chunkIdx, doublesCount);
    pDoubles.write(price);
    pDoubles.write(size);
    pDoubles.write(executedSize);
    pDoubles.write(tradePrice);
    pDoubles.write(tradeSize);

    return DxfgEventClazzT.DXFG_EVENT_ORDER.eventOrdinal();
  }

  /**
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L430
   * <p>
   * typedef struct dxfg_spread_order_t {
   * dxfg_order_base_t order_base;
   * const char* spread_symbol;
   * } dxfg_spread_order_t;
   *
   * @return
   */

  private static byte convertSpreadOrder(SpreadOrder event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                  // 8
    int eventFlags = event.getEventFlags();                                 // 4
    long index = event.getIndex();                                          // 8
    long timeSequence = event.getTimeSequence();                            // 8
    int timeNanoPart = event.getTimeNanoPart();                             // 4
    long actionTime = event.getActionTime();                                // 8
    long orderId = event.getOrderId();                                      // 8
    long auxOrderId = event.getAuxOrderId();                                // 8
    long count = event.getCount();                                          // 8
    int flags = DxFeedEventMarketPackagePrivate.getFlags(event);            // 4
    long tradeId = event.getTradeId();                                      // 8
    CString spreadSymbol = new CString(event.getSpreadSymbol());

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8) + (8) + (4) + (8) + (8) + (8) + (8) + (4) + (8)
        + spreadSymbol.totalAllocatedBytes;

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8
    pBytes.writeLong(timeSequence);             // 8
    pBytes.writeInt(timeNanoPart);              // 4
    pBytes.writeLong(actionTime);               // 8
    pBytes.writeLong(orderId);                  // 8
    pBytes.writeLong(auxOrderId);               // 8
    pBytes.writeLong(count);                    // 8
    pBytes.writeInt(flags);                     // 4
    pBytes.writeLong(tradeId);                  // 8
    pBytes.writeString(spreadSymbol);

    // DOUBLE DATA
    double price = event.getPrice();                // 1
    double size = event.getSize();                  // 1
    double executedSize = event.getExecutedSize();  // 1
    double tradePrice = event.getTradePrice();      // 1
    double tradeSize = event.getTradeSize();        // 1

    // DOUBLE DATA
    int doublesCount = 5;
    pDoubles.addChunk(chunkIdx, doublesCount);
    pDoubles.write(price);
    pDoubles.write(size);
    pDoubles.write(executedSize);
    pDoubles.write(tradePrice);
    pDoubles.write(tradeSize);

    return DxfgEventClazzT.DXFG_EVENT_SPREAD_ORDER.eventOrdinal();
  }

  /**
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L419
   *
   * typedef struct dxfg_analytic_order_t {
   *    dxfg_order_t order_base;
   *    double iceberg_peak_size;
   *    double iceberg_hidden_size;
   *    double iceberg_executed_size;
   *    int32_t iceberg_flags;
   * } dxfg_analytic_order_t;
   *
   */

  private static byte convertAnalyticsOrder(AnalyticOrder event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles,
                                            int chunkIdx)
  {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                      // 8
    int eventFlags = event.getEventFlags();                                     // 4
    long index = event.getIndex();                                              // 8
    long timeSequence = event.getTimeSequence();                                // 8
    int timeNanoPart = event.getTimeNanoPart();                                 // 4
    long actionTime = event.getActionTime();                                    // 8
    long orderId = event.getOrderId();                                          // 8
    long auxOrderId = event.getAuxOrderId();                                    // 8
    long count = event.getCount();                                              // 8
    int flags = DxFeedEventMarketPackagePrivate.getFlags(event);                // 4
    long tradeId = event.getTradeId();                                          // 8
    int icebergFlags = DxFeedEventMarketPackagePrivate.getIcebergFlags(event);  // 4

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8) + (8) + (4) + (8) + (8) + (8) + (8) + (4) + (8)
        + (4);

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8
    pBytes.writeLong(timeSequence);             // 8
    pBytes.writeInt(timeNanoPart);              // 4
    pBytes.writeLong(actionTime);               // 8
    pBytes.writeLong(orderId);                  // 8
    pBytes.writeLong(auxOrderId);               // 8
    pBytes.writeLong(count);                    // 8
    pBytes.writeInt(flags);                     // 4
    pBytes.writeLong(tradeId);                  // 8
    pBytes.writeInt(icebergFlags);              // 4

    // DOUBLE DATA
    double price = event.getPrice();                              // 1
    double size = event.getSize();                                // 1
    double executedSize = event.getExecutedSize();                // 1
    double tradePrice = event.getTradePrice();                    // 1
    double tradeSize = event.getTradeSize();                      // 1
    double icebergPeakSize = event.getIcebergPeakSize();          // 1
    double icebergHiddenSize = event.getIcebergHiddenSize();      // 1
    double icebergExecutedSize = event.getIcebergExecutedSize();  // 1

    // DOUBLE DATA
    int doublesCount = 8;
    pDoubles.addChunk(chunkIdx, doublesCount);
    pDoubles.write(price);
    pDoubles.write(size);
    pDoubles.write(executedSize);
    pDoubles.write(tradePrice);
    pDoubles.write(tradeSize);
    pDoubles.write(icebergPeakSize);
    pDoubles.write(icebergHiddenSize);
    pDoubles.write(icebergExecutedSize);

    return DxfgEventClazzT.DXFG_EVENT_ANALYTIC_ORDER.eventOrdinal();
  }
}
