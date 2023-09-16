// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_ORDERMAPPING_H
#define NATIVE_JNI_SDK_ORDERMAPPING_H

#include "api/dxfg_events.h"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed {
  using namespace jni;

  struct OrderMapping {
    static dxfg_order_base_t* toOrderBase(const char** pByteData, const double** pDoubleData);
    static dxfg_order_t* toOrder(const char** pByteData, const double** pDoubleData);
    static dxfg_analytic_order_t* toAnalyticsOrder(const char** pByteData, const double** pDoubleData);
    static dxfg_spread_order_t* toSpreadOrder(const char** pByteData, const double** pDoubleData);
    static void fromOrderBase(dxfg_order_base_t* eventType, ByteWriter& writer);
    static void fromSpreadOrder(dxfg_spread_order_t* eventType, ByteWriter& writer);
    static void fromOrder(dxfg_order_t* eventType, ByteWriter& writer);
    static void fromAnalyticOrder(dxfg_analytic_order_t* eventType, ByteWriter& writer);
  private:
    static void readOrderBase(const char** pByteData, const double** pDoubleData, dxfg_order_base_t* orderBase, dxfg_event_clazz_t eventType);
    static void writeOrderBase(dxfg_order_base_t* orderBase, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_ORDERMAPPING_H
