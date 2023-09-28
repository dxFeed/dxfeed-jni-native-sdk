// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_BYTEREADER_HPP
#define NATIVE_JNI_SDK_BYTEREADER_HPP

#include <cstdio>
#include <vector>

#include "api/dxfg_events.h"
#include "JNIUtils.hpp"

namespace dxfeed::jni {
  struct ByteReader {
    ByteReader(int size, const char* pByteData, const double* pDoubleData, const char* pEventTypes);

    int16_t readInt16_t();
    int32_t readInt();
    int64_t readLong();
    double readDouble();
    const char* readString();


    std::vector<dxfg_event_type_t*> toEvents();
  private:
    dxfg_event_clazz_t readEventType();
    uint8_t readUByte();

    template <typename PrimitiveT>
    PrimitiveT readPrimitive() {
      PrimitiveT value = 0;
      for (size_t n = 0; n < sizeof(PrimitiveT); ++n) {
        value <<= 8;
        value |= readUByte();
      }
      return value;
    }

    dxfg_event_type_t* toEvent(dxfg_event_clazz_t pEventTypes);

    const int size_;
    const char* pByteData_;
    const double* pDoubleData_;
    const char* pEventTypes_;
    std::vector<std::unique_ptr<const char*>> resources;
  };
}

#endif //NATIVE_JNI_SDK_BYTEREADER_HPP
