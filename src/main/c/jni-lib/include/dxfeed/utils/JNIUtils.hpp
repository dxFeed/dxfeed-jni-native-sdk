// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
#define DXFEED_JNI_NATIVE_SDK_JNIUTILS_H

#include "JNICommon.hpp"
#include "javah/com_dxfeed_api_JniTest.h"

namespace dxfeed::jni {
  const char JAVA_HOME[] = "JAVA_HOME";

  const char* getJavaHome(VMOptions* params);

  namespace nativeMethods {
    static JNINativeMethod methods[] = {
        {"nOnQuoteEventListener", "(I[B[D[BJ)V", (void*) &Java_com_dxfeed_api_JniTest_nOnQuoteEventListener},
    };
  }

  jclass safeFindClass(JNIEnv* env, const char* clazzName);

  void registerNativeMethods(JNIEnv* env, jclass clazz);

  typedef jmethodID (JNIEnv::*JMethodIdProvider)(jclass, const char*, const char*);
  jmethodID safeGetMethod(JNIEnv*, JMethodIdProvider, jclass, const char* methodName, const char* signature);
  jmethodID safeGetStaticMethodID(JNIEnv*, jclass, const char* methodName, const char* signature);
  jmethodID safeGetMethodID(JNIEnv* env, jclass, const char* methodName, const char* signature);
} // namespace dxfeed::jni

#endif //DXFEED_JNI_NATIVE_SDK_JNIUTILS_H