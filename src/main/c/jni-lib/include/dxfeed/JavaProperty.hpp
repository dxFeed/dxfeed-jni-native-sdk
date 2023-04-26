// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_PROPERTY_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_PROPERTY_H_

#include <jni.h>
#include <iostream>

namespace dxfeed::jni::internal {
  struct JavaProperty {
    JavaProperty(JNIEnv* env, jclass javaLangSystem, jmethodID getPropertyMethodId, const char* propName);
    ~JavaProperty();

    friend std::ostream& operator<<(std::ostream& os, const JavaProperty& prop) {
      if (prop.cstr_) {
        os << prop.cstr_;
      }
      return os;
    }
  private:
    jstring str_;
    JNIEnv* env_;
    const char* cstr_ = nullptr;
  };
}  // namespace dxfeed::jni::internal

#endif // DXFEED_JNI_NATIVE_SDK_JAVA_PROPERTY_H_