// SPDX-License-Identifier: MPL-2.0

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
