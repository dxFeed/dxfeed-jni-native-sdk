#include <jni.h>

#include "api/utils/JavaProperty.h"

namespace dxfeed::internal {
  JavaProperty::JavaProperty(jclass javaLangSystem, jmethodID getPropertyMethodId, const char* propName) {
    jstring property = jniEnv->NewStringUTF(propName);
    _str = reinterpret_cast<jstring>(jniEnv->CallStaticObjectMethod(javaLangSystem, getPropertyMethodId, property));
    if (_str) {
      _cstr = jniEnv->GetStringUTFChars(_str, nullptr);
    }
  }

  JavaProperty::~JavaProperty() {
    if (_str) {
      jniEnv->ReleaseStringUTFChars(_str, nullptr);
    }
  }
}