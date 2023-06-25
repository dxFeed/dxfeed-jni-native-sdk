// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEventT.hpp"

namespace dxfeed {
  DxEventT::DxEventT(dxfg_event_type_t eventType, const char* symbol) :
    eventType_(eventType),
    symbol_(symbol)
  {}

}
