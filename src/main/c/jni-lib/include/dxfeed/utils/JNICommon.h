// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_BASE_H_
#define DXFEED_JNI_NATIVE_SDK_BASE_H_

#include <jni.h>

namespace dxfeed::jni {
  JNIEnv* getJNIEnv();

  namespace internal {

    extern JNIEnv* jniEnv;
    extern JavaVM* javaVM;

    typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
    typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

    extern CreateJavaVM_t fCreateJavaVM;

    void initJavaVM(const char* javaHome, const char** vmOptions = nullptr, int vmArgsCount = 0);
  }
}

#endif //DXFEED_JNI_NATIVE_SDK_BASE_H_
