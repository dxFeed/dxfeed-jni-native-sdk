// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_
#define DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_

#include <jni.h>
#include <cstdint>

#include "CriticalSection.hpp"

namespace dxfeed::jni::internal::vm {
  struct JavaVmInstance {
    JavaVmInstance(JavaVM* vmPtr, int32_t jniVersion);
    ~JavaVmInstance();

    JNIEnv* getCurrenThread();
    bool attachCurrentThread(JNIEnv** env);
    void detachCurrentThread();
  private:
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
