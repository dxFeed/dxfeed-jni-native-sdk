#pragma once

#include <jni.h>
#include <iostream>

#include "../DxFeed.h"

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
}