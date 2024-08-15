// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_LANG_SYSTEM_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_LANG_SYSTEM_H_

#include <jni.h>

namespace dxfeed::jni {
struct JavaLangSystem {
    explicit JavaLangSystem(JNIEnv *env);

    const char *getProperty(JNIEnv *env, const char *key) const;
    void setProperty(JNIEnv *env, const char *key, const char *value);

  private:
    jmethodID loadMethodId = nullptr;
    jmethodID getPropMethodId = nullptr;
    jmethodID setPropMethodId = nullptr;
};

} // namespace dxfeed::jni
#endif // DXFEED_JNI_NATIVE_SDK_JAVA_LANG_SYSTEM_H_