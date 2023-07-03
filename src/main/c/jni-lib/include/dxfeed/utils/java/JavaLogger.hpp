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
    void info(const char* format, const T& value, const Targs&... Fargs) const {
      std::stringstream ss;
      logInfoFormatted(ss, format, value, Fargs...);
      info(ss.str());
    }

    template <class T, class ...Targs>
    void error(const char* format, const T& value, const Targs&... Fargs) const {
      std::stringstream ss;
      logInfoFormatted(ss, format, value, Fargs...);
      error(ss.str());
    }

    JavaLogger const& info(const std::string&) const;
    JavaLogger const& info(const char*) const;
    JavaLogger const& info(void const*) const;
    JavaLogger const& error(const std::string&) const;
    JavaLogger const& error(const char*) const;
    JavaLogger const& error(void const*) const;

  private:
    JavaLogger const& logInfo(const char* data) const;
    JavaLogger const& logError(const char* data) const;

    JNIEnv* env_;
    jobject logger_;
    jmethodID logInfo_;
    jmethodID logErr_;
  };
}
#endif // DXFEED_JNI_NATIVE_SDK_LOG_TO_JAVA_H