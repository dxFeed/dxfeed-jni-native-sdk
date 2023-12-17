// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <jni.h>

#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/UserDataSync.hpp"

using namespace dxfeed;

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
    fprintf(stderr,  "Error getting JNIEnv. Exiting...");
    return flag;
  }
  return JNI_VERSION_1_8;
}

JNIEXPORT
void JNI_OnUnload(JavaVM* vm, void* reserved) {
  fprintf(stdout, "JNI_OnUnload\n");
}

JNIEXPORT
void JNICALL Java_com_dxfeed_api_DxEndpointJni_nOnStateChangeListener(JNIEnv* env, jclass,
                                                                                  jint oldStateValue,
                                                                                  jint newStateValue,
                                                                                  jlong jUserCallback, jlong jUserData)
{
  auto userCallback = dxfeed::r_cast<dxfg_endpoint_state_change_listener_func>(jUserCallback);
  auto userData = dxfeed::r_cast<void*>(jUserData);
  auto oldState = static_cast<dxfg_endpoint_state_t>(oldStateValue);
  auto newState = static_cast<dxfg_endpoint_state_t>(newStateValue);
  userCallback(env, oldState, newState, userData);
}

JNIEXPORT
void JNICALL Java_com_dxfeed_api_DxSubscriptionJni_nOnEventListener(JNIEnv* env, jclass, jint size, jbyteArray jBytes,
                                                                    jdoubleArray jDoubles, jbyteArray jEventTypes,
                                                                    jlong jUserCallback, jlong jUserData)
{

/** Inside a critical region, native code must not call other JNI functions, or
 * any system call that may cause the current thread to block and wait for another Java thread.
 */
  user_data_sync::CONSUMER_PROCESSED_DATA.store(false);
  std::unique_lock<std::mutex> locker(user_data_sync::LOCK);

  jsize byteSize = env->GetArrayLength(jBytes);
  jsize doubleSize = env->GetArrayLength(jDoubles);
  jsize eventTypeSize = env->GetArrayLength(jEventTypes);

  auto pByteData = (const char*) env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (const double*) env->GetPrimitiveArrayCritical(jDoubles, nullptr);
  auto pEventTypes = (const char*) env->GetPrimitiveArrayCritical(jEventTypes, nullptr);

  user_data_sync::GLOBAL_BYTE_ARRAY.resize(byteSize);
  user_data_sync::GLOBAL_DOUBLE_ARRAY.resize(doubleSize);
  user_data_sync::GLOBAL_EVENT_TYPE_ARRAY.resize(eventTypeSize);
  std::copy(pByteData, pByteData + byteSize, user_data_sync::GLOBAL_BYTE_ARRAY.begin());
  std::copy(pDoubleData, pDoubleData + doubleSize, user_data_sync::GLOBAL_DOUBLE_ARRAY.begin());
  std::copy(pEventTypes, pEventTypes + eventTypeSize, user_data_sync::GLOBAL_EVENT_TYPE_ARRAY.begin());
  user_data_sync::GLOBAL_JAVA_USER_CALLBACK_ADDRESS = jUserCallback;
  user_data_sync::GLOBAL_JAVA_USER_DATA_ADDRESS = jUserData;

  env->ReleasePrimitiveArrayCritical(jDoubles, const_cast<double*>(pDoubleData), 0);
  env->ReleasePrimitiveArrayCritical(jBytes, const_cast<char*>(pByteData), 0);
  env->ReleasePrimitiveArrayCritical(jEventTypes, const_cast<char*>(pEventTypes), 0);

  user_data_sync::DATA_IS_READY.store(true);
  locker.unlock(); // Unlock after consumption.

  user_data_sync::CONDITION_VAR.notify_one(); // Notifies one waiting thread for the data
//  std::cout << "Producer : Blocked for the consumer." << std::endl;
  while (!user_data_sync::CONSUMER_PROCESSED_DATA.load()) {}
//  std::cout << "Producer : Released by the consumer." << std::endl;
}

/**
 * Don't use JavaCritical because we'd like to call JNI functions from user callback.
 * It's impossible for JavaCritical sections:
 *      https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html
 *
 *  Native frames: (J=compiled Java code, A=aot compiled Java code, j=interpreted, Vv=VM code, C=native code)
 *  V  [libjvm.dylib+0x75accd]  VMError::report_and_die(int, char const*, char const*, __va_list_tag*, Thread*, unsigned char*, void*, void*, char const*, int, unsigned long)+0x60d
 *  V  [libjvm.dylib+0x2672cf]  report_fatal(VMErrorType, char const*, int, char const*, ...)+0xc1
 *  V  [libjvm.dylib+0x686309]  SafepointSynchronize::block(JavaThread*)+0x1db
 *  V  [libjvm.dylib+0x688515]  SafepointMechanism::block_if_requested_slow(JavaThread*)+0x1d
 *  V  [libjvm.dylib+0x718283]  JavaThread::check_safepoint_and_suspend_for_native_trans(JavaThread*)+0x9f
 *  V  [libjvm.dylib+0x1d711d]  ThreadInVMfromNative::ThreadInVMfromNative(JavaThread*)+0x6d
 *  V  [libjvm.dylib+0x3d6e65]  jni_FindClass+0x45
 *  C  [native_jni.dylib+0x1b501]  JNIEnv_::FindClass(char const*)+0x21
 *  C  [native_jni.dylib+0x1b37a]  JavaCritical_com_dxfeed_api_DxSubscriptionJni_nOnEventListener+0x26a
 *
 *  */
JNIEXPORT
void JNICALL JavaCritical_com_dxfeed_api_DxSubscriptionJni_nOnEventListener(jint size,
                                                                            jint byteSize, jbyte* jBytes,
                                                                            jint doubleSize, jdouble* jDoubles,
                                                                            jint eventTypeSize, jbyte* jEventTypes,
                                                                            jlong jUserCallback, jlong jUserData)
{
  /** Inside a critical region, native code must not call other JNI functions, or
    * any system call that may cause the current thread to block and wait for another Java thread.
    */

  user_data_sync::CONSUMER_PROCESSED_DATA.store(false);
  std::unique_lock<std::mutex> locker(user_data_sync::LOCK);

  user_data_sync::GLOBAL_BYTE_ARRAY.resize(byteSize);
  user_data_sync::GLOBAL_DOUBLE_ARRAY.resize(doubleSize);
  user_data_sync::GLOBAL_EVENT_TYPE_ARRAY.resize(eventTypeSize);
  std::copy(jBytes, jBytes + byteSize, user_data_sync::GLOBAL_BYTE_ARRAY.begin());
  std::copy(jDoubles, jDoubles + doubleSize, user_data_sync::GLOBAL_DOUBLE_ARRAY.begin());
  std::copy(jEventTypes, jEventTypes + eventTypeSize, user_data_sync::GLOBAL_EVENT_TYPE_ARRAY.begin());
  user_data_sync::GLOBAL_JAVA_USER_CALLBACK_ADDRESS = jUserCallback;
  user_data_sync::GLOBAL_JAVA_USER_DATA_ADDRESS = jUserData;

  user_data_sync::DATA_IS_READY.store(true);
  locker.unlock(); // Unlock after consumption.

  user_data_sync::CONDITION_VAR.notify_one(); // Notifies one waiting thread for the data
//  std::cout << "Producer : Blocked for the consumer." << std::endl;
  while (!user_data_sync::CONSUMER_PROCESSED_DATA.load()) {}
//  std::cout << "Producer : Released by the consumer." << std::endl;
}

#ifdef __cplusplus
}
#endif