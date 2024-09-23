// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
#define DXFEED_JNI_NATIVE_SDK_JNIUTILS_H

#ifdef __cpp_lib_bit_cast
#    include <bit>
#endif

#include "JNICommon.hpp"

namespace dxfeed {
template <typename... T> constexpr void ignore_unused(const T &...) {
}

constexpr inline auto is_constant_evaluated(bool default_value = false) noexcept -> bool {
#ifdef __cpp_lib_is_constant_evaluated
    ignore_unused(default_value);
    return std::is_constant_evaluated();
#else
    return default_value;
#endif
}

// Implementation of std::bit_cast for pre-C++20.
template <typename To, typename From>
constexpr To bit_cast(const From &from)
#if __cpp_concepts
    requires(sizeof(To) == sizeof(From))
#endif
{
#ifdef __cpp_lib_bit_cast
    if (is_constant_evaluated())
        return std::bit_cast<To>(from);
#endif
    auto to = To();
    // The cast suppresses a bogus -Wclass-memaccess on GCC.
    std::memcpy(static_cast<void *>(&to), &from, sizeof(to));
    return to;
}

template <class TargetType, class InitialType> constexpr inline TargetType r_cast(InitialType arg) {
    return reinterpret_cast<TargetType>(arg);
}

namespace jni {
jclass safeFindClass(JNIEnv *env, const char *clazzName);

jmethodID safeGetStaticMethodID(JNIEnv *, jclass, const char *methodName, const char *signature);
jmethodID safeGetMethodID(JNIEnv *env, jclass, const char *methodName, const char *signature);
jfieldID safeGetStaticFieldID(JNIEnv *env, jclass clazz, const char *fieldName, const char *signature);
jfieldID safeGetFieldID(JNIEnv *env, jclass clazz, const char *fieldName, const char *signature);

// allocated new memory, for const char*, need to be deallocated manually
const char *jStringToUTF8(JNIEnv *env, jstring jString);
const char *copy(const char *str);

template <class... Args>
inline void checkedCallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID methodId, Args... args) {
    env->CallStaticVoidMethod(clazz, methodId, args...);
    auto pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
    }
}

template <class ReturnType, class... Args> // used when we'd like to cast jobject to ReturnType
inline typename std::enable_if_t<std::is_base_of_v<_jobject, std::remove_pointer_t<ReturnType>> ||
                                     std::is_base_of_v<_jarray, std::remove_pointer_t<ReturnType>>,
                                 ReturnType>
checkedCallStaticObjectMethodAndCastTo(JNIEnv *env, jclass clazz, jmethodID methodId, Args... args) {
    jobject result = env->CallStaticObjectMethod(clazz, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
        return nullptr;
    }
    return r_cast<ReturnType>(result);
}

template <class ReturnType, class... Args> // used when we'd like to cast jobject to ReturnType
inline typename std::enable_if_t<std::is_base_of_v<_jobject, std::remove_pointer_t<ReturnType>> ||
                                     std::is_base_of_v<_jarray, std::remove_pointer_t<ReturnType>>,
                                 ReturnType>
checkedCallObjectMethodAndCastTo(JNIEnv *env, jobject jObject, jmethodID methodId, Args... args) {
    jobject result = env->CallObjectMethod(jObject, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
        return nullptr;
    }
    return r_cast<ReturnType>(result);
}

template <class... Args> // used when we'd like to get jobject as is
inline jobject checkedCallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID methodId, Args... args) {
    jobject result = env->CallStaticObjectMethod(clazz, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
        return nullptr;
    }
    return result;
}

template <class... Args> // used when we'd like to get jobject as is
inline jobject checkedCallObjectMethod(JNIEnv *env, jobject jObject, jmethodID methodId, Args... args) {
    jobject result = env->CallObjectMethod(jObject, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
        return nullptr;
    }
    return result;
}

template <class... Args>
inline void checkedCallVoidMethod(JNIEnv *env, jobject jObject, jmethodID methodId, Args... args) {
    env->CallVoidMethod(jObject, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
    }
}

template <class... Args>
inline jlong checkedCallLongMethod(JNIEnv *env, jobject jObject, jmethodID methodId, Args... args) {
    auto result = env->CallLongMethod(jObject, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
        return 0;
    }
    return result;
}

template <class... Args>
inline jlong checkedCallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID methodId, Args... args) {
    jlong result = env->CallStaticLongMethod(clazz, methodId, args...);
    dxfg_exception_t *pException = dxfg_get_and_clear_thread_exception_t(env);
    if (pException) {
        dxfg_print_exception(env, pException);
        dxfg_Exception_release(env, pException);
        return 0;
    }
    return result;
}
} // namespace jni
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
