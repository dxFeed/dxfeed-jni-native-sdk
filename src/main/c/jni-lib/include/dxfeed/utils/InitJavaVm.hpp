// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_INTERNAL_H_
#define DXFEED_JNI_NATIVE_SDK_INTERNAL_H_

#include "JNICommon.hpp"

namespace dxfeed::jni::internal {

#if _MSC_VER && !__INTEL_COMPILER
  constexpr char JAR_SEPARATOR = ';';
  const char PATH_SEPARATOR = '\\';
  const char JNI_LIB_NAME[] = "native_jni.dll";
#else
  constexpr char JAR_SEPARATOR = ':';
  constexpr char PATH_SEPARATOR = '/';
  const char JNI_LIB_NAME[] = "native_jni.dylib";
#endif

  const char MY_JAR[] = "dxfeed-jni-native-sdk-0.0.1-SNAPSHOT.jar";
  const char* JARS[] = {
      "auther-api-442.jar",
      "dxfeed-api-3.313.jar",
      "dxlib-3.313.jar",
      "qds-3.313.jar",
      "qds-file-3.313.jar",
      "qds-tools-3.313.jar"
  };


  void loadJVMLibrary(const char*);
}  // namespace dxfeed::jni::internal

#endif // DXFEED_JNI_NATIVE_SDK_INTERNAL_H_