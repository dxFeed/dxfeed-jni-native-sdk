// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_INTERNAL_H_
#define DXFEED_JNI_NATIVE_SDK_INTERNAL_H_

#include <filesystem>
#include <jni.h>

namespace fs = std::filesystem;
namespace dxfeed::jni::internal {

  typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
  typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

  extern CreateJavaVM_t createJavaVM;

  void loadJVMLibrary(const char*);
  void loadJavaVM(const char* javaHome, const char** vmOptions = nullptr, int vmArgsCount = 0);
}  // namespace dxfeed::jni::internal

#endif // DXFEED_JNI_NATIVE_SDK_INTERNAL_H_