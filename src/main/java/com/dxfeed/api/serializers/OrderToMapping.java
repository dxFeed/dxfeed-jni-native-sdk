package com.dxfeed.api.serializers;

import com.dxfeed.api.DxfgEventClazzT;
import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.market.*;

public class OrderToMapping {
  public static byte toNative(OrderBase event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    if (event instanceof AnalyticOrder) {
      return convertAnalyticsOrder((AnalyticOrder) event, pBytes, pDoubles);
    } else if (event instanceof Order) {
      return convertOrder((Order)event, pBytes, pDoubles);
    } else if (event instanceof SpreadOrder) {
      return convertSpreadOrder((SpreadOrder) event, pBytes, pDoubles);
    } else {
      return writeOrderBase(event, pBytes, pDoubles);
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

  private static byte writeOrderBase(OrderBase event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                         // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                             // 8
    pBytes.writeInt(event.getEventFlags());                             // 4
    pBytes.writeLong(event.getIndex());                                 // 8
    pBytes.writeLong(event.getTimeSequence());                          // 8
    pBytes.writeInt(event.getTimeNanoPart());                           // 4
    pBytes.writeLong(event.getActionTime());                            // 8
    pBytes.writeLong(event.getOrderId());                               // 8
    pBytes.writeLong(event.getAuxOrderId());                            // 8
    pBytes.writeLong(event.getCount());                                 // 8
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getFlags(event));   // 4
    pBytes.writeLong(event.getTradeId());                               // 8

    // DOUBLE DATA
    pDoubles.write(event.getPrice());
    pDoubles.write(event.getSize());
    pDoubles.write(event.getExecutedSize());
    pDoubles.write(event.getTradePrice());
    pDoubles.write(event.getTradeSize());
    return DxfgEventClazzT.DXFG_EVENT_ORDER_BASE;
  }

  private static void orderBaseFromNative(OrderBase orderBase, NativeEventsReader reader) {
    orderBase.setEventSymbol(reader.readString());
    orderBase.setEventTime(reader.readLong());
    orderBase.setEventFlags(reader.readInt());
    orderBase.setIndex(reader.readLong());
    orderBase.setTimeSequence(reader.readLong());
    orderBase.setTimeNanoPart(reader.readInt());
    orderBase.setActionTime(reader.readLong());
    orderBase.setOrderId(reader.readLong());
    orderBase.setAuxOrderId(reader.readLong());
    orderBase.setCount(reader.readLong());
    DxFeedEventMarketPackagePrivate.setFlags(orderBase, reader.readInt());
    orderBase.setTradeId(reader.readLong());

    orderBase.setPrice(reader.readDouble());
    orderBase.setSizeAsDouble(reader.readDouble());
    orderBase.setExecutedSize(reader.readDouble());
    orderBase.setTradePrice(reader.readDouble());
    orderBase.setTradeSize(reader.readDouble());
  }

  public static OrderBase orderBaseFromNative(NativeEventsReader reader) {
    OrderBase orderBase = DxFeedEventMarketPackagePrivate.createOrderBase();
    orderBaseFromNative(orderBase, reader);
    return orderBase;
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

  private static byte convertOrder(Order event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    writeOrderBase(event, pBytes, pDoubles);
    pBytes.writeString(event.getMarketMaker());                         // 2 + marketMakerLength
    return DxfgEventClazzT.DXFG_EVENT_ORDER;
  }

  public static Order orderFromNative(NativeEventsReader reader) {
    Order order = new Order();
    orderBaseFromNative(order, reader);
    order.setMarketMaker(reader.readString());
    return order;
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

  private static byte convertSpreadOrder(SpreadOrder event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    writeOrderBase(event, pBytes, pDoubles);
    pBytes.writeString(event.getSpreadSymbol());                        // 2 + spreadSymbolLength
    return DxfgEventClazzT.DXFG_EVENT_SPREAD_ORDER;
  }

  public static SpreadOrder spreadOrderFromNative(NativeEventsReader reader) {
    SpreadOrder spreadOrder = new SpreadOrder();
    orderBaseFromNative(spreadOrder, reader);
    spreadOrder.setSpreadSymbol(reader.readString());
    return spreadOrder;
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

  private static byte convertAnalyticsOrder(AnalyticOrder event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    convertOrder(event, pBytes, pDoubles);
    // BYTE DATA
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getIcebergFlags(event));  // 4
    // DOUBLE DATA
    pDoubles.write(event.getIcebergPeakSize());
    pDoubles.write(event.getIcebergHiddenSize());
    pDoubles.write(event.getIcebergExecutedSize());

    return DxfgEventClazzT.DXFG_EVENT_ANALYTIC_ORDER;
  }

  public static AnalyticOrder analyticOrderFromNative(NativeEventsReader reader) {
    AnalyticOrder analyticOrder = new AnalyticOrder();
    orderBaseFromNative(analyticOrder, reader);
    // BYTE DATA
    DxFeedEventMarketPackagePrivate.setIcebergFlags(analyticOrder, reader.readInt());
    // DOUBLE DATA
    analyticOrder.setIcebergPeakSize(reader.readDouble());
    analyticOrder.setIcebergHiddenSize(reader.readDouble());
    analyticOrder.setIcebergExecutedSize(reader.readDouble());
    return analyticOrder;
  }
}
