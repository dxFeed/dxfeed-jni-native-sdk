// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <vector>
#include <iostream>

#include "javah/com_dxfeed_api_JniTest.h"
#include "api/Api.h"
#include "api/DxFeed.hpp"
#include "api/utils/Diagnostic.hpp"

template <typename T>
inline T readUByte(char** pData) {
  T val = (**pData) & 0xFF;
  ++(*pData);
  return val;
}

inline int8_t readByte(char** pData) {
  return readUByte<int8_t>(pData);
}

template <typename PrimitiveT>
inline PrimitiveT readPrimitive(char** pData) {
  PrimitiveT value = 0;
  for (size_t n = 0; n < sizeof(PrimitiveT); ++n) {
    value += readUByte<PrimitiveT>(pData) << (n * 8);
  }
  return value;
}

inline int16_t readInt16_t(char** pData) {
  return readPrimitive<int16_t>(pData);
}

inline int32_t readInt(char** pData) {
  return readPrimitive<int32_t>(pData);
}

inline int64_t readLong(char** pData) {
  return readPrimitive<int64_t>(pData);
}

inline double readDouble(double** pData) {
  double value = **pData;
  ++(*pData);
  return value;
}

#ifdef __cplusplus
extern "C" {
#endif

// https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html#JNJI_OnLoad
// -> register native method in created JNEEnv of VM when JNI lib is loaded
JNIEXPORT
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  JNIEnv* env;
  jint flag = vm->GetEnv((void**) &env, JNI_VERSION_1_8);
  if (flag == JNI_ERR) {
    std::cerr << "Error getting JNIEnv. Exiting..." << std::endl;
    return flag;
  }
  std::cout << "JNI_OnLoad, env = " << env << ", res = " << flag << std::endl;
  return JNI_VERSION_1_8;
}

JNIEXPORT
void JNI_OnUnload(JavaVM* vm, void* reserved) {
  std::cout << "JNI_OnUnload" << std::endl;
}

JNIEXPORT
void JNICALL Java_com_dxfeed_api_JniTest_nOnQuoteEventListener(JNIEnv* env, jclass, jint size,
                                                               jbyteArray jBytes, jdoubleArray jDoubles,
                                                               jlong userCallback) {
  std::vector<TimeAndSale> events(size);
  auto pByteData = (char*) env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (double*) env->GetPrimitiveArrayCritical(jDoubles, nullptr);

  for (auto& quote: events) {
    int16_t strSize = readInt16_t(&pByteData);
    quote.eventSymbol_ = pByteData;
    pByteData += strSize;
    quote.eventTime_ = readLong(&pByteData);
    quote.index_ = readLong(&pByteData);
    quote.eventFlags_ = readInt(&pByteData);
    quote.timeNanoPart_ = readInt(&pByteData);
    quote.exchangeCode_ = readByte(&pByteData);
    quote.size_ = readLong(&pByteData);

    strSize = readInt16_t(&pByteData);
    quote.exchangeSaleConditions_ = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote.buyer_ = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote.seller_ = strSize ? pByteData : "";
    pByteData += strSize;

    quote.price_ = readDouble(&pDoubleData);
    quote.bidPrice_ = readDouble(&pDoubleData);
    quote.askPrice_ = readDouble(&pDoubleData);
  }

  env->ReleasePrimitiveArrayCritical(jDoubles, pDoubleData, 0);
  env->ReleasePrimitiveArrayCritical(jBytes, pByteData, 0);
  //  const auto pListener = reinterpret_cast<dxfeed::perf::Receiver*>(userCallback);
  //  pListener->operator()(events);
  auto pListener = reinterpret_cast<dx_feed_listener>(userCallback);
  pListener(events.data(), size);
}

JNIEXPORT
void JNICALL JavaCritical_com_dxfeed_api_JniTest_nOnQuoteEventListener(jint size,
                                                                       jint byteLen, jbyte* jBytes, jint doubleLen,
                                                                       jdouble* jDoubles, jlong userCallback) {
  auto pByteData = (char*) jBytes;
  auto pDoubleData = (double*) jDoubles;
  std::vector<TimeAndSale> events(size);

  for (auto& quote: events) {
    int16_t strSize = readInt16_t(&pByteData);
    quote.eventSymbol_ = pByteData;
    pByteData += strSize;
    quote.eventTime_ = readLong(&pByteData);
    quote.index_ = readLong(&pByteData);
    quote.eventFlags_ = readInt(&pByteData);
    quote.timeNanoPart_ = readInt(&pByteData);
    quote.exchangeCode_ = readByte(&pByteData);
    quote.size_ = readLong(&pByteData);

    strSize = readInt16_t(&pByteData);
    quote.exchangeSaleConditions_ = pByteData;
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote.buyer_ = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote.seller_ = strSize ? pByteData : "";
    pByteData += strSize;

    quote.price_ = readDouble(&pDoubleData);
    quote.bidPrice_ = readDouble(&pDoubleData);
    quote.askPrice_ = readDouble(&pDoubleData);
  }
  //  const auto pListener = reinterpret_cast<dxfeed::perf::Receiver*>(userCallback);
  //  pListener->operator()(events);
  auto pListener = reinterpret_cast<dx_feed_listener>(userCallback);
  pListener(events.data(), size);
}

#ifdef __cplusplus
}
#endif