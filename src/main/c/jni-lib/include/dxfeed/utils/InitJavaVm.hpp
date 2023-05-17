// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_INTERNAL_H_
#define DXFEED_JNI_NATIVE_SDK_INTERNAL_H_

#include "JNICommon.hpp"

namespace dxfeed::jni::internal {

#if _MSC_VER && !__INTEL_COMPILER
  constexpr char JAR_SEPARATOR = ';';
  const char PATH_SEPARATOR = '\\';
#else
  constexpr char JAR_SEPARATOR = ':';
  constexpr char PATH_SEPARATOR = '/';
#endif
  extern char dllFilePath[];
  const char MY_JAR[] = "dxfeed-jni-native-sdk-0.1.0.jar";

  void loadJVMLibrary(const char*);
}  // namespace dxfeed::jni::internal

#endif // DXFEED_JNI_NATIVE_SDK_INTERNAL_H_