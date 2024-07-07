// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_BASE_H_
#define DXFEED_JNI_NATIVE_SDK_BASE_H_

#include <jni.h>
#include <cstring>
#include <memory>

#include "dxfeed/utils/DxThreadException.hpp"
#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/java/JavaLangSystem.hpp"
#include "dxfeed/utils/java/JavaLogger.hpp"
#include "dxfeed/utils/java/DxTimeFormat.hpp"
#include "dxfeed/utils/vm/JavaVmInstance.hpp"

namespace dxfeed::jni {
  typedef internal::vm::JavaVmInstance JVMInstance;

  struct VMOptions {
    const char* javaHome;
    const char** vmArgs;
    int vmArgsCount;
  };

  constexpr char JAVA_HOME[] = "JAVA_HOME";
#if _WIN32 || _WIN64
  constexpr char JAVA_CLASS_PATH_SEPARATOR[] = ";";
#else
  constexpr char JAVA_CLASS_PATH_SEPARATOR[] = ":";
#endif

  const char* getJavaHomeFromEnv();
  extern const JavaLogger* javaLogger;

  namespace internal {
    extern JNIEnv* jniEnv;
    extern JVMInstance* javaVM;
    extern JavaLangSystem* javaLangSystem;
    extern DxThreadException* dxThreadException;
    extern const JavaLangClass* javaLangClass;

    typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
    typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

    extern CreateJavaVM_t fCreateJavaVM;

    void initJavaVM(VMOptions* params);
  }
}

#endif //DXFEED_JNI_NATIVE_SDK_BASE_H_
