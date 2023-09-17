// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_ORDERMAPPING_H
#define NATIVE_JNI_SDK_ORDERMAPPING_H

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct OrderMapping {
    static dxfg_order_base_t* toOrderBase(ByteReader& reader);
    static dxfg_order_t* toOrder(ByteReader& reader);
    static dxfg_analytic_order_t* toAnalyticsOrder(ByteReader& reader);
    static dxfg_spread_order_t* toSpreadOrder(ByteReader& reader);
    static void fromOrderBase(dxfg_order_base_t* eventType, ByteWriter& writer);
    static void fromSpreadOrder(dxfg_spread_order_t* eventType, ByteWriter& writer);
    static void fromOrder(dxfg_order_t* eventType, ByteWriter& writer);
    static void fromAnalyticOrder(dxfg_analytic_order_t* eventType, ByteWriter& writer);
  private:
    static void readOrderBase(ByteReader& reader, dxfg_order_base_t* orderBase);
    static void writeOrderBase(dxfg_order_base_t* orderBase, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_ORDERMAPPING_H
