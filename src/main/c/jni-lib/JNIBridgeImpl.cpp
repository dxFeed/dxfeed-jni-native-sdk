// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <vector>
#include <iostream>

#include "javah/com_dxfeed_api_JniTest.h"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/NativeEventReader.hpp"

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
void JNICALL Java_com_dxfeed_api_JniTest_nOnStateChangeListener(JNIEnv* env, jclass,
                                                               jint oldStateValue, jint newStateValue,
                                                               jlong userCallback)
{
  auto pListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(userCallback);
  pListener->callUserFunc(env, oldStateValue, newStateValue);
}

JNIEXPORT
void JNICALL Java_com_dxfeed_api_JniTest_nOnQuoteEventListener(JNIEnv* env, jclass, jint size,
                                                               jbyteArray jBytes, jdoubleArray jDoubles,
                                                               jbyteArray jEventTypes,
                                                               jlong userCallback)
{
  auto pByteData = (char*) env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (double*) env->GetPrimitiveArrayCritical(jDoubles, nullptr);
  auto pEventTypes = (char*) env->GetPrimitiveArrayCritical(jEventTypes, nullptr);

  const auto& events = dxfeed::jni::NativeEventReader::toEvents(size, pByteData, pDoubleData, pEventTypes);

  env->ReleasePrimitiveArrayCritical(jDoubles, pDoubleData, 0);
  env->ReleasePrimitiveArrayCritical(jBytes, pByteData, 0);
  env->ReleasePrimitiveArrayCritical(jEventTypes, pEventTypes, 0);

  auto pListener = reinterpret_cast<dxfeed::DxEventListener*>(userCallback);
  dxfg_event_type_list list = { size, events.data() };
  pListener->callUserFunc(nullptr, &list); // todo: discuss thread == nullptr?
  for (const auto& event : events) {
    delete event;
  }
}

JNIEXPORT
void JNICALL JavaCritical_com_dxfeed_api_JniTest_nOnQuoteEventListener(jint size,
                                                                       jint byteLen, jbyte* jBytes,
                                                                       jint doubleLen, jdouble* jDoubles,
                                                                       jint eventTypesLen, jbyte* jEventTypes,
                                                                       jlong userCallback)
{
  auto pByteData = (char*) jBytes;
  auto pDoubleData = (double*) jDoubles;
  auto pEventTypes = (char*) jEventTypes;

  const auto& events = dxfeed::jni::NativeEventReader::toEvents(size, pByteData, pDoubleData, pEventTypes);

  auto pListener = reinterpret_cast<dxfeed::DxEventListener*>(userCallback);
  dxfg_event_type_list list = { size, events.data() };
  pListener->callUserFunc(nullptr, &list); // todo: discuss thread == nullptr?

  for (const auto& event : events) {
    delete[] event;
  }
}

#ifdef __cplusplus
}
#endif