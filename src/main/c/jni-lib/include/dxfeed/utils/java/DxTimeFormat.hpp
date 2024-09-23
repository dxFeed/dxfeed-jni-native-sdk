// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_TIME_FORMAT_CLASS_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_TIME_FORMAT_CLASS_H_

#include <jni.h>
#include <mutex>
#include <string>

namespace dxfeed::jni {
struct DxTimeFormat {
    static void init(JNIEnv *env);

    static DxTimeFormat *getDefault();
    static DxTimeFormat *getGMT();

    DxTimeFormat *withTimeZone(JNIEnv *env);
    DxTimeFormat *withMillis(JNIEnv *env);
    DxTimeFormat *asFullIso(JNIEnv *env);
    int64_t parse(JNIEnv *env, const char *);
    const char *format(JNIEnv *env, jlong);

  private:
    DxTimeFormat(JNIEnv *env, jobject tf);
    ~DxTimeFormat();
    jobject timeFormat_;
    static jobject initDefault(JNIEnv *env, jclass);

    static jobject initGMT(JNIEnv *env, jclass);
    inline static jmethodID withTimeZoneMethodId_;

    inline static jmethodID withMillisMethodId_;
    inline static jmethodID asFullIsoMethodId_;
    inline static jmethodID parseMethodId_;
    inline static jmethodID formatMethodId_;
    // long java.util.Date::getTime();
    inline static jmethodID dataGetTimeId_;

    inline static DxTimeFormat *DEFAULT_;
    inline static DxTimeFormat *GMT_;
};

} // namespace dxfeed::jni
#endif // DXFEED_JNI_NATIVE_SDK_JAVA_TIME_FORMAT_CLASS_H_