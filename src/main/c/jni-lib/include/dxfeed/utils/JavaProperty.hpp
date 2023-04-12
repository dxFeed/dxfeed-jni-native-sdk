// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_PROPERTY_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_PROPERTY_H_

#include <jni.h>
#include <iostream>

#include "dxfeed/DxFeed.hpp"

namespace dxfeed::internal {
  struct JavaProperty {
    JavaProperty(jclass javaLangSystem, jmethodID getPropertyMethodId, const char* propName);
    ~JavaProperty();

    friend std::ostream& operator<<(std::ostream& os, const JavaProperty& prop) {
      if (prop.cstr_) {
        os << prop.cstr_;
      }
      return os;
    }
  private:
    const char* cstr_ = nullptr;
    jstring str_;
  };
}  // namespace dxfeed::internal

#endif // DXFEED_JNI_NATIVE_SDK_JAVA_PROPERTY_H_