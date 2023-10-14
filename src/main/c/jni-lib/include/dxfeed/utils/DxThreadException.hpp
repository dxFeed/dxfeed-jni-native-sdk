// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_DXTHREADEXCEPTION_HPP
#define NATIVE_JNI_SDK_DXTHREADEXCEPTION_HPP

#include "api/dxfg_catch_exception.h"

namespace dxfeed::jni {
  struct DxThreadException {
    explicit DxThreadException(JNIEnv* env);
    ~DxThreadException();
    dxfg_exception_t* getAndClearThreadException(JNIEnv* env);
    jclass getJniClass();
  private:
    jclass dxThreadExceptionClass_;
    jmethodID getExceptionInfo_;
  };
}

#endif //NATIVE_JNI_SDK_DXTHREADEXCEPTION_HPP
