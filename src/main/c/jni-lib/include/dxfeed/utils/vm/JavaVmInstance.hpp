// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_
#define DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_

#include <jni.h>
#include <cstdint>

#include "CriticalSection.hpp"

namespace dxfeed::jni::internal::vm {
  struct JavaVmInstance {
    static JavaVmInstance* getInstance(JavaVM* vmPtr, int32_t jniVersion);

    JavaVmInstance(const JavaVmInstance& other) = delete;
    JavaVmInstance(JavaVmInstance&& other) = delete;
    JavaVmInstance& operator=(const JavaVmInstance& other) = delete;
    JavaVmInstance& operator=(JavaVmInstance&& other) = delete;

    JNIEnv* getCurrenThread();
    int32_t attachCurrentThread(JNIEnv** env);
    void detachCurrentThread();
  private:
    JavaVmInstance(JavaVM* vmPtr, int32_t jniVersion);
    ~JavaVmInstance();
    static const char* hrToMsg(int hr);
    static void logErrMsg(JNIEnv* env, jint hr, const char* errMsg);
    static void logHRESULT(JNIEnv* env, jint hr);

    JavaVM* vm_ = nullptr;
    const jint jniVersion_;
    CriticalSection criticalSection_;

    struct DetachJniThreadOnExit {
      explicit DetachJniThreadOnExit(JavaVM* vm);
      ~DetachJniThreadOnExit();
    private:
      JavaVM* vmCached_;
    };
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_
