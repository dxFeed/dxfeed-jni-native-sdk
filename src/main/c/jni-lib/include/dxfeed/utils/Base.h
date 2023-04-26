//
// Created by elmon on 26.04.2023.
//

#ifndef NATIVE_JNI_SDK_BASE_H
#define NATIVE_JNI_SDK_BASE_H

#include <jni.h>

namespace dxfeed::jni::internal {
  typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
  typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

  extern CreateJavaVM_t createJavaVM;
  void initJavaVM(const char* javaHome, const char** vmOptions = nullptr, int vmArgsCount = 0);
}

#endif //NATIVE_JNI_SDK_BASE_H
