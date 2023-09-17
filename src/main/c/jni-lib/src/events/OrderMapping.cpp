// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/OrderMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_order_base_t* OrderMapping::toOrderBase(const char** pByteData, const double** pDoubleData) {
    auto* orderBase = new dxfg_order_base_t();
    readOrderBase(pByteData, pDoubleData, orderBase);
    return orderBase;
  }

  void OrderMapping::fromOrderBase(dxfg_order_base_t* orderBase, ByteWriter& writer) {
    writeOrderBase(orderBase, writer);
  }

  dxfg_spread_order_t* OrderMapping::toSpreadOrder(const char** pByteData, const double** pDoubleData) {
    auto* order = new dxfg_spread_order_t();
    readOrderBase(pByteData, pDoubleData, &order->order_base);
    order->spread_symbol = ByteReader::readString(pByteData);
    return order;
  }

  void OrderMapping::fromSpreadOrder(dxfg_spread_order_t* order, ByteWriter& writer) {
    fromOrderBase(&order->order_base, writer);
    writer.writeString(order->spread_symbol);
  }

  dxfg_order_t* OrderMapping::toOrder(const char** pByteData, const double** pDoubleData) {
    auto* order = new dxfg_order_t();
    readOrderBase(pByteData, pDoubleData, &order->order_base);
    order->market_maker = ByteReader::readString(pByteData);
    return order;
  }

  void OrderMapping::fromOrder(dxfg_order_t* order, ByteWriter& writer) {
    fromOrderBase(&order->order_base, writer);
    writer.writeString(order->market_maker);
  }

  dxfg_analytic_order_t* OrderMapping::toAnalyticsOrder(const char** pByteData, const double** pDoubleData) {
    auto* analyticsOrder = new dxfg_analytic_order_t();
    dxfg_order_base_t& orderBase = analyticsOrder->order.order_base;
    readOrderBase(pByteData, pDoubleData, &orderBase);

    analyticsOrder->order.market_maker = ByteReader::readString(pByteData);
    analyticsOrder->iceberg_flags = ByteReader::readInt(pByteData);
    analyticsOrder->iceberg_peak_size = ByteReader::readDouble(pDoubleData);
    analyticsOrder->iceberg_hidden_size = ByteReader::readDouble(pDoubleData);
    analyticsOrder->iceberg_executed_size = ByteReader::readDouble(pDoubleData);
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
                                   dxfg_order_base_t* const orderBase)
  {
    orderBase->market_event.event_type.clazz = DXFG_EVENT_ORDER_BASE;
    orderBase->market_event.event_symbol = ByteReader::readString(pByteData);
    orderBase->market_event.event_time = ByteReader::readLong(pByteData);
    orderBase->event_flags = ByteReader::readInt(pByteData);
    orderBase->index = ByteReader::readLong(pByteData);
    orderBase->time_sequence = ByteReader::readLong(pByteData);
    orderBase->time_nano_part = ByteReader::readInt(pByteData);
    orderBase->action_time = ByteReader::readLong(pByteData);
    orderBase->order_id = ByteReader::readLong(pByteData);
    orderBase->aux_order_id = ByteReader::readLong(pByteData);
    orderBase->count = ByteReader::readLong(pByteData);
    orderBase->flags = ByteReader::readInt(pByteData);
    orderBase->trade_id = ByteReader::readLong(pByteData);

    orderBase->price = ByteReader::readDouble(pDoubleData);
    orderBase->size = ByteReader::readDouble(pDoubleData);
    orderBase->executed_size = ByteReader::readDouble(pDoubleData);
    orderBase->trade_price = ByteReader::readDouble(pDoubleData);
    orderBase->trade_size = ByteReader::readDouble(pDoubleData);
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