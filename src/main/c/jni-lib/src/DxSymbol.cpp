// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxSymol.hpp"

namespace dxfeed {
  DxSymbol::DxSymbol(dxfg_symbol_type_t eventType, jlong nativeHandlerId) :
    eventType(eventType),
    nativeHandlerId(nativeHandlerId)
  {}
}
