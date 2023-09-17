// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/OrderMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_order_base_t* OrderMapping::toOrderBase(ByteReader& reader) {
    auto* orderBase = new dxfg_order_base_t();
    readOrderBase(reader, orderBase);
    return orderBase;
  }

  void OrderMapping::fromOrderBase(dxfg_order_base_t* orderBase, ByteWriter& writer) {
    writeOrderBase(orderBase, writer);
  }

  dxfg_spread_order_t* OrderMapping::toSpreadOrder(ByteReader& reader) {
    auto* order = new dxfg_spread_order_t();
    readOrderBase(reader, &order->order_base);
    order->spread_symbol = reader.readString();
    return order;
  }

  void OrderMapping::fromSpreadOrder(dxfg_spread_order_t* order, ByteWriter& writer) {
    fromOrderBase(&order->order_base, writer);
    writer.writeString(order->spread_symbol);
  }

  dxfg_order_t* OrderMapping::toOrder(ByteReader& reader) {
    auto* order = new dxfg_order_t();
    readOrderBase(reader, &order->order_base);
    order->market_maker = reader.readString();
    return order;
  }

  void OrderMapping::fromOrder(dxfg_order_t* order, ByteWriter& writer) {
    fromOrderBase(&order->order_base, writer);
    writer.writeString(order->market_maker);
  }

  dxfg_analytic_order_t* OrderMapping::toAnalyticsOrder(ByteReader& reader) {
    auto* analyticsOrder = new dxfg_analytic_order_t();
    dxfg_order_base_t& orderBase = analyticsOrder->order.order_base;
    readOrderBase(reader, &orderBase);

    analyticsOrder->order.market_maker = reader.readString();
    analyticsOrder->iceberg_flags = reader.readInt();
    analyticsOrder->iceberg_peak_size = reader.readDouble();
    analyticsOrder->iceberg_hidden_size = reader.readDouble();
    analyticsOrder->iceberg_executed_size = reader.readDouble();
    return analyticsOrder;
  }

  void OrderMapping::fromAnalyticOrder(dxfg_analytic_order_t* analyticsOrder, ByteWriter& writer) {
    fromOrder(&analyticsOrder->order, writer);
    writer.writeInt32_t(analyticsOrder->iceberg_flags);
    writer.writeDouble(analyticsOrder->iceberg_peak_size);
    writer.writeDouble(analyticsOrder->iceberg_hidden_size);
    writer.writeDouble(analyticsOrder->iceberg_executed_size);
  }

  void OrderMapping::readOrderBase(ByteReader& reader, dxfg_order_base_t* const orderBase) {
    orderBase->market_event.event_type.clazz = DXFG_EVENT_ORDER_BASE;
    orderBase->market_event.event_symbol = reader.readString();
    orderBase->market_event.event_time = reader.readLong();
    orderBase->event_flags = reader.readInt();
    orderBase->index = reader.readLong();
    orderBase->time_sequence = reader.readLong();
    orderBase->time_nano_part = reader.readInt();
    orderBase->action_time = reader.readLong();
    orderBase->order_id = reader.readLong();
    orderBase->aux_order_id = reader.readLong();
    orderBase->count = reader.readLong();
    orderBase->flags = reader.readInt();
    orderBase->trade_id = reader.readLong();

    orderBase->price = reader.readDouble();
    orderBase->size = reader.readDouble();
    orderBase->executed_size = reader.readDouble();
    orderBase->trade_price = reader.readDouble();
    orderBase->trade_size = reader.readDouble();
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