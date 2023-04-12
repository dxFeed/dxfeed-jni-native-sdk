// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "api/utils/JavaProperty.hpp"

namespace dxfeed::internal {
  JavaProperty::JavaProperty(jclass javaLangSystem, jmethodID getPropertyMethodId, const char* propName) {
    jstring property = jniEnv->NewStringUTF(propName);
    str_ = reinterpret_cast<jstring>(jniEnv->CallStaticObjectMethod(javaLangSystem, getPropertyMethodId, property));
    if (str_) {
      cstr_ = jniEnv->GetStringUTFChars(str_, nullptr);
    }
  }

  JavaProperty::~JavaProperty() {
    if (str_) {
      jniEnv->ReleaseStringUTFChars(str_, nullptr);
    }
  }
}