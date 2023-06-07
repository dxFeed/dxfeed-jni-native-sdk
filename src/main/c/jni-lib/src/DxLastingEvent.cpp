// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxLastingEvent.hpp"

namespace dxfeed {
  DxLastingEvent::DxLastingEvent(dxfg_event_type_t eventType, jlong nativeHandlerId) :
    eventType(eventType),
    nativeHandlerId(nativeHandlerId)
  {}

}
