// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/OrderMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_order_base_t* OrderMapping::toOrderBase(const char** pByteData, const double** pDoubleData) {
    auto* orderBase = new dxfg_order_base_t();
    readOrderBase(pByteData, pDoubleData, orderBase, DXFG_EVENT_ORDER_BASE);
    return orderBase;
  }

  void OrderMapping::fromOrderBase(dxfg_order_base_t* orderBase, ByteWriter& writer) {
    writeOrderBase(orderBase, writer);
  }

  dxfg_spread_order_t* OrderMapping::toSpreadOrder(const char** pByteData, const double** pDoubleData) {
    auto* order = new dxfg_spread_order_t();
    readOrderBase(pByteData, pDoubleData, &order->order_base, DXFG_EVENT_SPREAD_ORDER);
    order->spread_symbol = EventReader::readString(pByteData);
    return order;
  }

  void OrderMapping::fromSpreadOrder(dxfg_spread_order_t* order, ByteWriter& writer) {
    fromOrderBase(&order->order_base, writer);
    writer.writeString(order->spread_symbol);
  }

  dxfg_order_t* OrderMapping::toOrder(const char** pByteData, const double** pDoubleData) {
    auto* order = new dxfg_order_t();
    readOrderBase(pByteData, pDoubleData, &order->order_base, DXFG_EVENT_ORDER);
    order->market_maker = EventReader::readString(pByteData);
    return order;
  }

  void OrderMapping::fromOrder(dxfg_order_t* order, ByteWriter& writer) {
    fromOrderBase(&order->order_base, writer);
    writer.writeString(order->market_maker);
  }

  dxfg_analytic_order_t* OrderMapping::toAnalyticsOrder(const char** pByteData, const double** pDoubleData) {
    auto* analyticsOrder = new dxfg_analytic_order_t();
    dxfg_order_base_t& orderBase = analyticsOrder->order.order_base;
    readOrderBase(pByteData, pDoubleData, &orderBase, DXFG_EVENT_ANALYTIC_ORDER);

    analyticsOrder->order.market_maker = EventReader::readString(pByteData);
    analyticsOrder->iceberg_flags = EventReader::readInt(pByteData);
    analyticsOrder->iceberg_peak_size = EventReader::readDouble(pDoubleData);
    analyticsOrder->iceberg_hidden_size = EventReader::readDouble(pDoubleData);
    analyticsOrder->iceberg_executed_size = EventReader::readDouble(pDoubleData);
    return analyticsOrder;
  }

  void OrderMapping::fromAnalyticOrder(dxfg_analytic_order_t* analyticsOrder, ByteWriter& writer) {
    fromOrder(&analyticsOrder->order, writer);
    writer.writeInt32_t(analyticsOrder->iceberg_flags);
    writer.writeDouble(analyticsOrder->iceberg_peak_size);
    writer.writeDouble(analyticsOrder->iceberg_hidden_size);
    writer.writeDouble(analyticsOrder->iceberg_executed_size);
  }

  void OrderMapping::readOrderBase(const char** pByteData,
                                   const double** pDoubleData,
                                   dxfg_order_base_t* const orderBase, dxfg_event_clazz_t eventType)
  {
    orderBase->market_event.event_type.clazz = eventType;
    orderBase->market_event.event_symbol = EventReader::readString(pByteData);
    orderBase->market_event.event_time = EventReader::readLong(pByteData);
    orderBase->event_flags = EventReader::readInt(pByteData);
    orderBase->index = EventReader::readLong(pByteData);
    orderBase->time_sequence = EventReader::readLong(pByteData);
    orderBase->time_nano_part = EventReader::readInt(pByteData);
    orderBase->action_time = EventReader::readLong(pByteData);
    orderBase->order_id = EventReader::readLong(pByteData);
    orderBase->aux_order_id = EventReader::readLong(pByteData);
    orderBase->count = EventReader::readLong(pByteData);
    orderBase->flags = EventReader::readInt(pByteData);
    orderBase->trade_id = EventReader::readLong(pByteData);

    orderBase->price = EventReader::readDouble(pDoubleData);
    orderBase->size = EventReader::readDouble(pDoubleData);
    orderBase->executed_size = EventReader::readDouble(pDoubleData);
    orderBase->trade_price = EventReader::readDouble(pDoubleData);
    orderBase->trade_size = EventReader::readDouble(pDoubleData);
  }

  void OrderMapping::writeOrderBase(dxfg_order_base_t* orderBase, ByteWriter& writer) {
    writer.writeString(orderBase->market_event.event_symbol);
    writer.writeInt64_t(orderBase->market_event.event_time);
    writer.writeInt32_t(orderBase->event_flags);
    writer.writeInt64_t(orderBase->index);
    writer.writeInt64_t(orderBase->time_sequence);
    writer.writeInt32_t(orderBase->time_nano_part);
    writer.writeInt64_t(orderBase->action_time);
    writer.writeInt64_t(orderBase->order_id);
    writer.writeInt64_t(orderBase->aux_order_id);
    writer.writeInt64_t(orderBase->count);
    writer.writeInt32_t(orderBase->flags);
    writer.writeInt64_t(orderBase->trade_id);

    writer.writeDouble(orderBase->price);
    writer.writeDouble(orderBase->size);
    writer.writeDouble(orderBase->executed_size);
    writer.writeDouble(orderBase->trade_price);
    writer.writeDouble(orderBase->trade_size);
  }
}