// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_BASE_H_
#define DXFEED_JNI_NATIVE_SDK_BASE_H_

#include <jni.h>

#include "dxfeed/utils/vm/JavaVmInstance.hpp"
#include "dxfeed/utils/java/JavaLangSystem.hpp"
#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/java/JavaLogger.hpp"

namespace dxfeed::jni {
  typedef internal::vm::JavaVmInstance JVMInstance;

  struct VMOptions {
    const char* javaHome;
    const char** vmArgs;
    int vmArgsCount;
  };

  constexpr char JAVA_HOME[] = "JAVA_HOME";
  const char* getJavaHomeFromEnv();
  extern const JavaLogger* javaLogger;

  namespace internal {
    extern JNIEnv* jniEnv;
    extern JVMInstance* javaVM;
    extern JavaLangSystem* javaLangSystem;
    extern const JavaLangClass* javaLangClass;

    typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
    typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

    extern CreateJavaVM_t fCreateJavaVM;

    void initJavaVM(VMOptions* params);
  }
}

#endif //DXFEED_JNI_NATIVE_SDK_BASE_H_
