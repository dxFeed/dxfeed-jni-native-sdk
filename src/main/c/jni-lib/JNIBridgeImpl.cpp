// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <vector>
#include <iostream>

#include "javah/com_dxfeed_api_JniTest.h"
#include "dxfeed/DxEventListener.hpp"

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
                                                               jbyteArray pEventTypes,
                                                               jlong userCallback)
{
  std::vector<dxfg_event_type_t*> events(size);
  auto pByteData = (char*) env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (double*) env->GetPrimitiveArrayCritical(jDoubles, nullptr);

  // todo: read event based on pEventType
  for (int i = 0 ; i < size; ++i) {
    auto* quote = new dxfg_time_and_sale_t();
    events[i] = reinterpret_cast<dxfg_event_type_t*>(quote);

    quote->market_event.event_type = {dxfg_event_clazz_t::DXFG_EVENT_TIME_AND_SALE};
    int16_t strSize = readInt16_t(&pByteData);
    quote->market_event.event_symbol = pByteData;
    pByteData += strSize;
    quote->market_event.event_time = readLong(&pByteData);
    quote->index = readLong(&pByteData);
    quote->event_flags = readInt(&pByteData);
    quote->time_nano_part = readInt(&pByteData);
    quote->exchange_code = readByte(&pByteData); // todo: check type
    quote->size = readLong(&pByteData); // todo: check type

    strSize = readInt16_t(&pByteData);
    quote->exchange_sale_conditions = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote->buyer = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote->seller = strSize ? pByteData : "";
    pByteData += strSize;

    quote->price = readDouble(&pDoubleData);
    quote->bid_price = readDouble(&pDoubleData);
    quote->ask_price = readDouble(&pDoubleData);
  }

  env->ReleasePrimitiveArrayCritical(jDoubles, pDoubleData, 0);
  env->ReleasePrimitiveArrayCritical(jBytes, pByteData, 0);

  auto pListener = reinterpret_cast<dxfeed::DxEventListener*>(userCallback);
  dxfg_event_type_list list = { size, events.data() };
  pListener->callUserFunc(nullptr, &list);
}

JNIEXPORT
void JNICALL JavaCritical_com_dxfeed_api_JniTest_nOnQuoteEventListener(jint size,
                                                                       jint byteLen, jbyte* jBytes,
                                                                       jint doubleLen, jdouble* jDoubles,
                                                                       jint eventTypesLen, jbyte* pEventTypes,
                                                                       jlong userCallback)
{
  auto pByteData = (char*) jBytes;
  auto pDoubleData = (double*) jDoubles;
  std::vector<dxfg_event_type_t*> events(size);

  for (int i = 0 ; i < size; ++i) {
    auto* quote = new dxfg_time_and_sale_t();
    events[i] = reinterpret_cast<dxfg_event_type_t*>(quote);

    quote->market_event.event_type =  { dxfg_event_clazz_t::DXFG_EVENT_TIME_AND_SALE };
    int16_t strSize = readInt16_t(&pByteData);
    quote->market_event.event_symbol = pByteData;
    pByteData += strSize;
    quote->market_event.event_time = readLong(&pByteData);
    quote->index = readLong(&pByteData);
    quote->event_flags = readInt(&pByteData);
    quote->time_nano_part = readInt(&pByteData);
    quote->exchange_code = readByte(&pByteData); // todo: check type
    quote->size = readLong(&pByteData); // todo: check type

    strSize = readInt16_t(&pByteData);
    quote->exchange_sale_conditions = pByteData;
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote->buyer = strSize ? pByteData : "";
    pByteData += strSize;

    strSize = readInt16_t(&pByteData);
    quote->seller = strSize ? pByteData : "";
    pByteData += strSize;

    quote->price = readDouble(&pDoubleData);
    quote->bid_price = readDouble(&pDoubleData);
    quote->ask_price = readDouble(&pDoubleData);
  }

  auto pListener = reinterpret_cast<dxfeed::DxEventListener*>(userCallback);
  dxfg_event_type_list list = { size, events.data() };
  pListener->callUserFunc(nullptr, &list);
}

#ifdef __cplusplus
}
#endif