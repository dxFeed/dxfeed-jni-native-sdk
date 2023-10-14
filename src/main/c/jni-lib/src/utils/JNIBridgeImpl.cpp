// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

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
  fprintf(stdout, "JNI_OnLoad, env = %, res = %\n", env, flag);
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
  auto pByteData = (const char*) env->GetPrimitiveArrayCritical(jBytes, nullptr);
  auto pDoubleData = (const double*) env->GetPrimitiveArrayCritical(jDoubles, nullptr);
  auto pEventTypes = (const char*) env->GetPrimitiveArrayCritical(jEventTypes, nullptr);

  dxfeed::jni::ByteReader reader(size, pByteData, pDoubleData, pEventTypes);
  auto events = reader.toEvents();

  env->ReleasePrimitiveArrayCritical(jDoubles, const_cast<double*>(pDoubleData), 0);
  env->ReleasePrimitiveArrayCritical(jBytes, const_cast<char*>(pByteData), 0);
  env->ReleasePrimitiveArrayCritical(jEventTypes, const_cast<char*>(pEventTypes), 0);

  auto pListener = dxfeed::r_cast<dxfg_feed_event_listener_function>(jUserCallback);
  auto userData = dxfeed::r_cast<void*>(jUserData);
  dxfg_event_type_list list = { size, events.data() };

  pListener(env, &list, userData);

  for (const auto& event : events) {
    delete event;
  }
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

  auto pByteData = (const char*) jBytes;
  auto pDoubleData = (const double*) jDoubles;
  auto pEventTypes = (const char*) jEventTypes;

  dxfeed::jni::ByteReader reader(size, pByteData, pDoubleData, pEventTypes);
  auto events = reader.toEvents();
  
  auto pListener = dxfeed::r_cast<dxfg_feed_event_listener_function>(jUserCallback);
  auto userData = dxfeed::r_cast<void*>(jUserData);
  dxfg_event_type_list list = { size, events.data() };

  JNIEnv* thread = dxfeed::jni::internal::javaVM->getCurrenThread();
  pListener(thread, &list, userData);
  for (const auto& event : events) {
    delete event;
  }
  dxfeed::jni::internal::javaVM->detachCurrentThread();
}

#ifdef __cplusplus
}
#endif