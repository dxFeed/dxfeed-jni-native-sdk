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

  JavaLogger::JavaLogger(JNIEnv* env) :
    env_(env)
  {
    jclass dxFeedJniClazz = env->FindClass("Lcom/dxfeed/api/DxFeedJni;");
    jfieldID fieldId = env->GetStaticFieldID(dxFeedJniClazz, "logger", "Lcom/devexperts/logging/Logging;");
    logger_ = env->NewGlobalRef(env->GetStaticObjectField(dxFeedJniClazz, fieldId));
    jclass serrClazz = env->GetObjectClass(logger_);
    logInfo_ = jni::safeGetMethodID(env_, serrClazz, "info", "(Ljava/lang/String;)V");
    logErr_ = jni::safeGetMethodID(env_, serrClazz, "error", "(Ljava/lang/String;)V");
    env->DeleteLocalRef(serrClazz);
    env->DeleteLocalRef(dxFeedJniClazz);
  }

  JavaLogger const& JavaLogger::info(const std::string& str) const {
    return logInfo(str.c_str());
  }

  JavaLogger const& JavaLogger::info(const char* str) const {
    return logInfo(str);
  }

  JavaLogger const& JavaLogger::info(void const* ptr) const {
    auto hexStr = toHex64(r_cast<uint64_t>(ptr));
    return logInfo(hexStr.c_str());
  }

  JavaLogger const& JavaLogger::error(const std::string& str) const {
    return logError(str.c_str());
  }

  JavaLogger const& JavaLogger::error(const char* str) const {
    return logError(str);
  }

  JavaLogger const& JavaLogger::error(void const* ptr) const {
    auto hexStr = toHex64(r_cast<uint64_t>(ptr));
    return logError(hexStr.c_str());
  }

  JavaLogger const& JavaLogger::logInfo(const char* data) const {
    jstring jString = env_->NewStringUTF(data);
    env_->CallVoidMethod(logger_, logInfo_, jString);
    env_->DeleteLocalRef(jString);
    return *this;
  }

  JavaLogger const& JavaLogger::logError(const char* data) const {
    jstring jString = env_->NewStringUTF(data);
    env_->CallVoidMethod(logger_, logErr_, jString);
    env_->DeleteLocalRef(jString);
    return *this;
  }
}