// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_LANG_CLASS_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_LANG_CLASS_H_

#include <jni.h>
#include <string>

namespace dxfeed::jni {
struct JavaLangClass {
    explicit JavaLangClass(JNIEnv *env);

    std::string getName(JNIEnv *env, jobject jObject) const;
    std::string getName(JNIEnv *env, jclass clazzObject) const;

  private:
    jmethodID getNameMethodId = nullptr;
};

} // namespace dxfeed::jni
#endif // DXFEED_JNI_NATIVE_SDK_JAVA_LANG_CLASS_H_