// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/JavaLogger.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
std::string toHex64(uint64_t uLongPtr) {
    std::string hex;
    while (uLongPtr > 0) {
        char r = static_cast<char>(uLongPtr % 16);
        char c = (r < 10) ? ('0' + r) : ('a' + r - 10);
        hex += c;
        uLongPtr /= 16;
    }
    hex = "0x" + hex;
    return hex;
}

JavaLogger::JavaLogger(JNIEnv *env) {
    auto jDevexLoggingClazz = safeFindClass(env, "com/devexperts/logging/Logging");
    auto id = jni::safeGetStaticMethodID(env, jDevexLoggingClazz, "getLogging",
                                         "(Ljava/lang/String;)"
                                         "Lcom/devexperts/logging/Logging;");
    auto pJstring = env->NewStringUTF("NativeLogger");
    logger_ = env->NewGlobalRef(checkedCallStaticObjectMethod(env, jDevexLoggingClazz, id, pJstring));
    env->DeleteLocalRef(pJstring);
    env->DeleteLocalRef(jDevexLoggingClazz);

    auto serrClazz = env->GetObjectClass(logger_);
    logInfo_ = jni::safeGetMethodID(env, serrClazz, "info", "(Ljava/lang/String;)V");
    logTrace_ = jni::safeGetMethodID(env, serrClazz, "trace", "(Ljava/lang/String;)V");
    logErr_ = jni::safeGetMethodID(env, serrClazz, "error", "(Ljava/lang/String;)V");
    env->DeleteLocalRef(serrClazz);
}

JavaLogger const &JavaLogger::info(JNIEnv *env, const std::string &str) const {
    return logInfo(env, str.c_str());
}

JavaLogger const &JavaLogger::info(JNIEnv *env, const char *str) const {
    return logInfo(env, str);
}

JavaLogger const &JavaLogger::info(JNIEnv *env, void const *ptr) const {
    auto hexStr = toHex64(r_cast<uint64_t>(ptr));
    return logInfo(env, hexStr.c_str());
}

JavaLogger const &JavaLogger::trace(JNIEnv *env, const std::string &str) const {
    return logTrace(env, str.c_str());
}

JavaLogger const &JavaLogger::trace(JNIEnv *env, const char *str) const {
    return logTrace(env, str);
}

JavaLogger const &JavaLogger::trace(JNIEnv *env, void const *ptr) const {
    auto hexStr = toHex64(r_cast<uint64_t>(ptr));
    return logTrace(env, hexStr.c_str());
}

JavaLogger const &JavaLogger::error(JNIEnv *env, const std::string &str) const {
    return logError(env, str.c_str());
}

JavaLogger const &JavaLogger::error(JNIEnv *env, const char *str) const {
    return logError(env, str);
}

JavaLogger const &JavaLogger::error(JNIEnv *env, void const *ptr) const {
    auto hexStr = toHex64(r_cast<uint64_t>(ptr));
    return logError(env, hexStr.c_str());
}

JavaLogger const &JavaLogger::logInfo(JNIEnv *env, const char *data) const {
    auto jString = env->NewStringUTF(data);
    checkedCallVoidMethod(env, logger_, logInfo_, jString);
    env->DeleteLocalRef(jString);
    return *this;
}

JavaLogger const &JavaLogger::logTrace(JNIEnv *env, const char *data) const {
    auto jString = env->NewStringUTF(data);
    checkedCallVoidMethod(env, logger_, logTrace_, jString);
    env->DeleteLocalRef(jString);
    return *this;
}

JavaLogger const &JavaLogger::logError(JNIEnv *env, const char *data) const {
    auto jString = env->NewStringUTF(data);
    checkedCallVoidMethod(env, logger_, logErr_, jString);
    env->DeleteLocalRef(jString);
    return *this;
}
} // namespace dxfeed::jni