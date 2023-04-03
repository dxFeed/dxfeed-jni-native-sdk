#include <jni.h>
#include <vector>
#include <iostream>

#include "headers/com_dxfeed_api_JniTest.h"
#include "api/DxFeed.h"
#include "api/utils/Diagnostic.h"

template <typename T>
inline T readUByte(char** pData) {
  T val = (**pData) & 0xFF;
  ++(*pData);
  return val;
}

inline int8_t readByte(char** pData) {
  return readUByte<int8_t>(pData);
}

inline int32_t readInt(char** pData) {
  int32_t value = 0;
  for (int n = 0; n < sizeof(int32_t); ++n) {
    value += readUByte<int32_t>(pData) << (n * 8);
  }
  return value;
}

inline int64_t readLong(char** pData) {
  int64_t value = 0;
  for (int n = 0; n < sizeof(int64_t); ++n) {
    value += readUByte<int64_t>(pData) << (n * 8);
  }
  return value;
}

inline double readDouble(double** pData) {
  double value = **pData;
  ++(*pData);
  return value;
}

extern "C" {

// https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html#JNJI_OnLoad
// -> register native method in created JNEEnv of VM when JNI lib is loaded
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv* env;
  jint flag = vm->GetEnv((void**)&env, JNI_VERSION_1_8);
  if (flag == JNI_ERR) {
    std::cerr << "Error getting JNIEnv. Exiting..." << std::endl;
    return flag;
  }
  std::cout << "JNI_OnLoad, env = " << env << ", res = " << flag << std::endl;
  return JNI_VERSION_1_8;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
  std::cout << "JNI_OnUnload" << std::endl;
}

JNIEXPORT
void JNICALL Java_com_dxfeed_api_JniTest_nOnQuoteEventListener(JNIEnv* env, jclass, jint size,
                                                               jbyteArray jBytes, jdoubleArray jDoubles, jlong userCallback)
{
  std::vector<TimeAndSale> events(size);
  auto pByteData = (char *)env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (double *)env->GetPrimitiveArrayCritical(jDoubles, nullptr);

  for (auto& quote : events) {
    int strSize = readByte(&pByteData);
    quote.event_symbol = pByteData;
    pByteData += strSize;
    quote.event_time = readLong(&pByteData);
    quote.index = readLong(&pByteData);
    quote.event_flags = readInt(&pByteData);
    quote.time_nano_part = readInt(&pByteData);
    quote.exchange_code = readByte(&pByteData);
    quote.size = readLong(&pByteData);

    strSize = readByte(&pByteData);
    quote.exchangeSaleConditions = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readByte(&pByteData);
    quote.buyer = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readByte(&pByteData);
    quote.seller = strSize ? pByteData : "";
    pByteData += strSize;

    quote.price = readDouble(&pDoubleData);
    quote.bid_price = readDouble(&pDoubleData);
    quote.ask_price = readDouble(&pDoubleData);
  }

  env->ReleasePrimitiveArrayCritical(jDoubles, pDoubleData, 0);
  env->ReleasePrimitiveArrayCritical(jBytes, pByteData, 0);
  const auto pListener = reinterpret_cast<dxfeed::perf::Receiver*>(userCallback);
  pListener->operator()(events);
}

JNIEXPORT
void JNICALL JavaCritical_com_dxfeed_api_JniTest_nOnQuoteEventListener(jint size,
                                                                       jint byteLen, jbyte* jBytes, jint doubleLen,
                                                                       jdouble* jDoubles, jlong userCallback)
{
  auto pByteData = (char*)jBytes;
  auto pDoubleData = (double*)jDoubles;
//  std::cout << "Critical " << std::endl;
//  std::cout << "\tsize = " << size << std::endl;
//  std::cout << "\tbyteLen = " << byteLen << std::endl;
//  std::cout << "\tjBytes = " << jBytes << std::endl;
//  std::cout << "\tdoubleLen = " << doubleLen << std::endl;
//  std::cout << "\tjDoubles = " << jDoubles << std::endl;
  std::vector<TimeAndSale> events(size);

  for (auto& quote : events) {
    int strSize = readByte(&pByteData);
    quote.event_symbol = pByteData;
    pByteData += strSize;
    quote.event_time = readLong(&pByteData);
    quote.index = readLong(&pByteData);
    quote.event_flags = readInt(&pByteData);
    quote.time_nano_part = readInt(&pByteData);
    quote.exchange_code = readByte(&pByteData);
    quote.size = readLong(&pByteData);

    strSize = readByte(&pByteData);
    quote.exchangeSaleConditions = pByteData;
    pByteData += strSize;

    strSize = readByte(&pByteData);
    quote.buyer = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readByte(&pByteData);
    quote.seller = strSize ? pByteData : "";
    pByteData += strSize;

    quote.price = readDouble(&pDoubleData);
    quote.bid_price = readDouble(&pDoubleData);
    quote.ask_price = readDouble(&pDoubleData);
  }

  const auto pListener = reinterpret_cast<dxfeed::perf::Receiver*>(userCallback);
  pListener->operator()(events);
}
}