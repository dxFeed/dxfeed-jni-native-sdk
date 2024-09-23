// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_
#define DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_

#include <cstdint>
#include <jni.h>

#include "CriticalSection.hpp"

namespace dxfeed::jni::internal::vm {
struct JavaVmInstance {
    static JavaVmInstance *getInstance(JavaVM *vmPtr, int32_t jniVersion);

    JavaVmInstance(const JavaVmInstance &other) = delete;
    JavaVmInstance(JavaVmInstance &&other) = delete;
    JavaVmInstance &operator=(const JavaVmInstance &other) = delete;
    JavaVmInstance &operator=(JavaVmInstance &&other) = delete;

    JNIEnv *getCurrenThread();
    int32_t attachCurrentThread(JNIEnv **env);
    void detachCurrentThread();

  private:
    JavaVmInstance(JavaVM *vmPtr, int32_t jniVersion);
    ~JavaVmInstance();
    static const char *hrToMsg(int hr);
    static void logErrMsg(JNIEnv *env, jint hr, const char *errMsg);
    static void logHRESULT(JNIEnv *env, jint hr);

    JavaVM *vm_ = nullptr;
    const jint jniVersion_;
    // TODO: replace by std::recursive_mutex
    CriticalSection criticalSection_;

    struct DetachJniThreadOnExit {
        explicit DetachJniThreadOnExit(JavaVM *vm);
        ~DetachJniThreadOnExit();

      private:
        JavaVM *vmCached_;
    };
};
} // namespace dxfeed::jni::internal::vm

#endif // DXFEED_JNI_NATIVE_SDK_JAVAVM_INSTANCE_HPP_H_
