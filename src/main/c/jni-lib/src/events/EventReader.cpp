// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/EventReader.h"

namespace dxfeed {
  template <typename T>
  inline T readUByte(const char** pData) {
    T val = (**pData) & 0xFF;
    ++(*pData);
    return val;
  }

  template <typename PrimitiveT>
  inline PrimitiveT readPrimitive(const char** pData) {
    PrimitiveT value = 0;
    for (size_t n = 0; n < sizeof(PrimitiveT); ++n) {
      value |= readUByte<PrimitiveT>(pData) << (n * 8);
    }
    return value;
  }

  int8_t EventReader::readByte(const char** pData) {
    return readUByte<int8_t>(pData);
  }

  int16_t EventReader::readInt16_t(const char** pData) {
    return readPrimitive<int16_t>(pData);
  }

  int32_t EventReader::readInt(const char** pData) {
    return readPrimitive<int32_t>(pData);
  }

  int64_t EventReader::readLong(const char** pData) {
    return readPrimitive<int64_t>(pData);
  }

  double EventReader::readDouble(const double** pData) {
    double value = **pData;
    ++(*pData);
    return value;
  }

  const char* EventReader::readString(const char** pData) {
    int16_t strSize = readInt16_t(pData);
    const auto result = strSize ? *pData : "";
    (*pData) += strSize;
    return result;
  }
}