// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_NATIVEEVENTWRITER_H_
#define DXFEED_JNI_NATIVE_SDK_NATIVEEVENTWRITER_H_

#include <vector>

#include "api/dxfg_events.h"

namespace dxfeed::jni {
  struct ByteWriter {
    explicit ByteWriter(int eventCount);
    ~ByteWriter();
    void writeEvent(dxfg_event_type_t* eventType);

    jbyteArray eventTypes(JNIEnv* env);
    jbyteArray byteData(JNIEnv* env);
    jdoubleArray doubleData(JNIEnv* env);

    void writeString(const char* str);
    void writeInt64_t(int64_t value);
    void writeInt32_t(int32_t value);
    void writeInt16_t(int16_t value);
    void writeDouble(double value);

  private:
    inline void writeBytes(const int8_t* bytes, int32_t len);

    std::vector<uint8_t> eventTypes_;
    std::vector<uint8_t> byteData_;
    std::vector<double> doubleData_;
    static const int EVENT_SIZE_IN_BYTES = 128;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_NATIVEEVENTWRITER_H_
