// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_EVENTREADER_H
#define NATIVE_JNI_SDK_EVENTREADER_H

#include <cstdio>

namespace dxfeed {

  struct EventReader {
    static int8_t readByte(const char** pData);
    static int16_t readInt16_t(const char** pData);
    static int32_t readInt(const char** pData);
    static int64_t readLong(const char** pData);
    static double readDouble(const double** pData);
    static const char* readString(const char** pData);
  };
}

#endif //NATIVE_JNI_SDK_EVENTREADER_H
