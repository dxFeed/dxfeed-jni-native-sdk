// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
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
void JNICALL Java_com_dxfeed_api_DxEndpointJni_nOnStateChangeListener(JNIEnv* env, jclass,
                                                                                  jint oldStateValue,
                                                                                  jint newStateValue,
                                                                                  jlong jUserCallback, jlong jUserData)
{
  auto userCallback = reinterpret_cast<dxfg_endpoint_state_change_listener_func>(jUserCallback);
  auto userData = reinterpret_cast<void*>(jUserData);
  auto oldState = static_cast<dxfg_endpoint_state_t>(oldStateValue);
  auto newState = static_cast<dxfg_endpoint_state_t>(newStateValue);
  userCallback(env, oldState, newState, userData);
}

JNIEXPORT
void JNICALL Java_com_dxfeed_api_DxSubscriptionJni_nOnEventListener(JNIEnv* env, jclass, jint size, jbyteArray jBytes,
                                                                    jdoubleArray jDoubles, jbyteArray jEventTypes,
                                                                    jlong jUserCallback, jlong jUserData)
{
  auto pByteData = (const char*) env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (const double*) env->GetPrimitiveArrayCritical(jDoubles, nullptr);
  auto pEventTypes = (const char*) env->GetPrimitiveArrayCritical(jEventTypes, nullptr);

  auto events = dxfeed::jni::NativeEventReader::toEvents(size, pByteData, pDoubleData, pEventTypes);

  env->ReleasePrimitiveArrayCritical(jDoubles, const_cast<double*>(pDoubleData), 0);
  env->ReleasePrimitiveArrayCritical(jBytes, const_cast<char*>(pByteData), 0);
  env->ReleasePrimitiveArrayCritical(jEventTypes, const_cast<char*>(pEventTypes), 0);

  auto pListener = reinterpret_cast<dxfg_feed_event_listener_function>(jUserCallback);
  auto userData = reinterpret_cast<void*>(jUserData);
  dxfg_event_type_list list = { size, events.data() };
  pListener(nullptr, &list, userData); // todo: discuss thread == nullptr?
  for (const auto& event : events) {
    delete event;
  }
}

JNIEXPORT
void JNICALL JavaCritical_com_dxfeed_api_DxSubscriptionJni_nOnEventListener(jint size, jint byteLen, jbyte* jBytes,
                                                                            jint doubleLen, jdouble* jDoubles,
                                                                            jint eventTypesLen, jbyte* jEventTypes,
                                                                            jlong jUserCallback, jlong jUserData)
{
  auto pByteData = (char*) jBytes;
  auto pDoubleData = (double*) jDoubles;
  auto pEventTypes = (char*) jEventTypes;

  auto events = dxfeed::jni::NativeEventReader::toEvents(size, pByteData, pDoubleData, pEventTypes);

  auto pListener = reinterpret_cast<dxfg_feed_event_listener_function>(jUserCallback);
  auto userData = reinterpret_cast<void*>(jUserData);
  dxfg_event_type_list list = { size, events.data() };
  pListener(nullptr, &list, userData); // todo: discuss thread == nullptr?

  for (const auto& event : events) {
    delete[] event;
  }
}

#ifdef __cplusplus
}
#endif