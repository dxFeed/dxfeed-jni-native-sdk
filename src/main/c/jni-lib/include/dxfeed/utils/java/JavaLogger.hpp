// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_LOG_TO_JAVA_H
#define DXFEED_JNI_NATIVE_SDK_LOG_TO_JAVA_H

#include <jni.h>
#include <sstream>

namespace dxfeed::jni {

  class JavaLogger {
    template <class T>
    void logInfoFormatted(std::stringstream& ss, const T& arg) const {
      ss << arg;
    }

    template <class T, class ...Targs>
    void logInfoFormatted(std::stringstream& ss, const char* format, const T& value, const Targs&... Fargs) const {
      for (; *format != '\0'; format++) {
        if (*format == '%') {
          ss << value;
          logInfoFormatted(ss, format + 1, Fargs...); // recursive call
          return;
        }
        ss << *format;
      }
    }
  public:
    explicit JavaLogger(JNIEnv* env);

    template <class T, class ...Targs>
    void info(JNIEnv* env, const char* format, const T& value, const Targs&... Fargs) const {
      std::stringstream ss;
      logInfoFormatted(ss, format, value, Fargs...);
      info(env, ss.str());
    }

    template <class T, class ...Targs>
    void error(JNIEnv* env, const char* format, const T& value, const Targs&... Fargs) const {
      std::stringstream ss;
      logInfoFormatted(ss, format, value, Fargs...);
      error(env, ss.str());
    }

    JavaLogger const& info(JNIEnv* env, const std::string&) const;
    JavaLogger const& info(JNIEnv* env, const char*) const;
    JavaLogger const& info(JNIEnv* env, void const*) const;
    JavaLogger const& error(JNIEnv* env, const std::string&) const;
    JavaLogger const& error(JNIEnv* env, const char*) const;
    JavaLogger const& error(JNIEnv* env, void const*) const;

  private:
    JavaLogger const& logInfo(JNIEnv* env, const char* data) const;
    JavaLogger const& logError(JNIEnv* env, const char* data) const;

    jobject logger_;
    jmethodID logInfo_;
    jmethodID logErr_;
  };
}
#endif // DXFEED_JNI_NATIVE_SDK_LOG_TO_JAVA_H