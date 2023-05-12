// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_BASE_H_
#define DXFEED_JNI_NATIVE_SDK_BASE_H_

#include <jni.h>
#include "dxfeed/utils/vm/JavaVmInstance.hpp"
#include "dxfeed/utils/java/JavaLangSystem.hpp"

namespace dxfeed::jni {
  typedef internal::vm::JavaVmInstance JVMInstance;

  struct VMOptions {
    const char* javaHome;
    const char** vmArgs;
    int vmArgsCount;
  };

  namespace internal {
    extern JNIEnv* jniEnv;
    extern JVMInstance* javaVM;
    extern const JavaLangSystem* javaLangSystem;

    typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
    typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

    extern CreateJavaVM_t fCreateJavaVM;

    void initJavaVM(VMOptions* params);
  }
}

#endif //DXFEED_JNI_NATIVE_SDK_BASE_H_
