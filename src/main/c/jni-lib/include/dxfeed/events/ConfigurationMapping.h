// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_CONFIGURATIONMAPPING_H
#define NATIVE_JNI_SDK_CONFIGURATIONMAPPING_H

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteWriter;

  struct ConfigurationMapping {
    static dxfg_configuration_t* toConfiguration(const char** pByteData);
    static void fromConfiguration(dxfg_configuration_t* eventType, ByteWriter& writer);
  };
}

#endif //NATIVE_JNI_SDK_CONFIGURATIONMAPPING_H
