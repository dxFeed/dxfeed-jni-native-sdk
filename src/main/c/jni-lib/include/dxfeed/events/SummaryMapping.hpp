// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NATIVE_JNI_SDK_SUMMARYMAPPING_HPP
#define NATIVE_JNI_SDK_SUMMARYMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct SummaryMapping {
    static dxfg_summary_t* toSummary(ByteReader& reader);
    static void fromSummary(dxfg_summary_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_SUMMARYMAPPING_HPP
