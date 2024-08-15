// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_BASE_H_
#define DXFEED_JNI_NATIVE_SDK_BASE_H_

#include <cstring>
#include <jni.h>
#include <limits>
#include <memory>

#include "dxfeed/utils/DxThreadException.hpp"
#include "dxfeed/utils/java/DxJni.hpp"
#include "dxfeed/utils/java/DxTimeFormat.hpp"
#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/java/JavaLangSystem.hpp"
#include "dxfeed/utils/java/JavaLogger.hpp"
#include "dxfeed/utils/vm/JavaVmInstance.hpp"

#define INVALID_INT_RESULT_MINUS_ONE (-1)
#define INVALID_INT_RESULT_MINUS_MIN (-(std::numeric_limits<std::int32_t>::min)())
#define INVALID_PTR_RESULT_NULLPTR (nullptr)
#define INVALID_DOUBLE_RESULT_NEG_INF (-std::numeric_limits<double>::infinity())

namespace dxfeed::jni {
typedef internal::vm::JavaVmInstance JVMInstance;

struct VMOptions {
    const char *javaHome;
    const char **vmArgs;
    int vmArgsCount;
};

constexpr char JAVA_HOME[] = "JAVA_HOME";
#if _WIN32 || _WIN64
constexpr char JAVA_CLASS_PATH_SEPARATOR[] = ";";
#else
constexpr char JAVA_CLASS_PATH_SEPARATOR[] = ":";
#endif

const char *getJavaHomeFromEnv();
extern const JavaLogger *javaLogger;

namespace internal {
extern JNIEnv *jniEnv;
extern JVMInstance *javaVM;
extern JavaLangSystem *javaLangSystem;
extern DxThreadException *dxThreadException;
extern const JavaLangClass *javaLangClass;

typedef jint(JNICALL *CreateJavaVM_t)(JavaVM **pvm, void **env, void *args);
typedef jclass(JNICALL *FindClassFromBootLoader_t)(JNIEnv *env, const char *name);

extern CreateJavaVM_t fCreateJavaVM;

void initJavaVM(VMOptions *params);
} // namespace internal
} // namespace dxfeed::jni

#endif // DXFEED_JNI_NATIVE_SDK_BASE_H_
