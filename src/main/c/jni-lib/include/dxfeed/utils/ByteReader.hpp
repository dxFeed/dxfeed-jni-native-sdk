// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_BYTEREADER_HPP
#define NATIVE_JNI_SDK_BYTEREADER_HPP

#include <cstdio>

namespace dxfeed::jni {
  struct ByteReader {
    static int8_t readByte(const char** pData);
    static int16_t readInt16_t(const char** pData);
    static int32_t readInt(const char** pData);
    static int64_t readLong(const char** pData);
    static double readDouble(const double** pData);
    static const char* readString(const char** pData);
  };
}

#endif //NATIVE_JNI_SDK_BYTEREADER_HPP
