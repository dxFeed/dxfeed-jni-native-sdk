// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
#define DXFEED_JNI_NATIVE_SDK_JNIUTILS_H

#include "JNICommon.hpp"

namespace dxfeed {
  template <typename OutputType, typename InputType>
  OutputType r_cast(InputType t) {
    return reinterpret_cast<OutputType>(t);
  }

  namespace jni {
    jclass safeFindClass(JNIEnv* env, const char* clazzName);

    typedef jmethodID (JNIEnv::*JMethodIdProvider)(jclass, const char*, const char*);
    jmethodID safeGetStaticMethodID(JNIEnv*, jclass, const char* methodName, const char* signature);
    jmethodID safeGetMethodID(JNIEnv* env, jclass, const char* methodName, const char* signature);
  }
} // namespace dxfeed::jni

#endif //DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
