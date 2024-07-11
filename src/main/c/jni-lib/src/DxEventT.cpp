// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/DxEventT.hpp"

namespace dxfeed {
  DxEventT::DxEventT(dxfg_event_type_t eventType, const char* symbol) :
    eventType_(eventType),
    symbol_(symbol)
  {}

}
