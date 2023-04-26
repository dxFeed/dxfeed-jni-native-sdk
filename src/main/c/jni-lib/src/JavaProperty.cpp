// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/JavaProperty.hpp"

namespace dxfeed::jni::internal {
  JavaProperty::JavaProperty(JNIEnv* env,jclass javaLangSystem, jmethodID getPropertyMethodId, const char* propName) :
      env_(env)
  {
    jstring property = env_->NewStringUTF(propName);
    str_ = reinterpret_cast<jstring>(env_->CallStaticObjectMethod(javaLangSystem, getPropertyMethodId, property));
    if (str_) {
      cstr_ = env_->GetStringUTFChars(str_, nullptr);
    }
  }

  JavaProperty::~JavaProperty() {
    if (str_) {
      env_->ReleaseStringUTFChars(str_, nullptr);
      env_->DeleteLocalRef(str_);
    }
    env_ = nullptr;
  }
}