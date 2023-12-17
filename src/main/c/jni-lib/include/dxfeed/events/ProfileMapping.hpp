// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NATIVE_JNI_SDK_PROFILEMAPPING_HPP
#define NATIVE_JNI_SDK_PROFILEMAPPING_HPP

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteReader;
  struct ByteWriter;

  struct ProfileMapping {
    static dxfg_profile_t* toProfile(ByteReader& reader);
    static void fromProfile(dxfg_profile_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_PROFILEMAPPING_HPP
