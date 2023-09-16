// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_NATIVEEVENTREADER_H_
#define DXFEED_JNI_NATIVE_SDK_NATIVEEVENTREADER_H_

#include <vector>

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct NativeEventReader {
    static std::vector<dxfg_event_type_t*> toEvents(int size, const char* pByteData, const double* pDoubleData, const char* pEventTypes);
  private:
    static dxfg_event_type_t* toEvent(const char** pByteData, const double** pDoubleData, dxfg_event_clazz_t pEventTypes);
    static dxfg_configuration_t* toConfiguration(const char** pByteData);
    static dxfg_message_t* toMessage(const char** pByteData);
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_NATIVEEVENTREADER_H_
