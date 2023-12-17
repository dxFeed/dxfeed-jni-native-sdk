// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NATIVE_JNI_SDK_DXTHREADEXCEPTION_HPP
#define NATIVE_JNI_SDK_DXTHREADEXCEPTION_HPP

#include "api/dxfg_catch_exception.h"

namespace dxfeed::jni {
  struct DxThreadException {
    explicit DxThreadException(JNIEnv* env);
    ~DxThreadException();
    dxfg_exception_t* getAndClearThreadException(JNIEnv* env);
  private:
    jclass dxThreadExceptionClass_;
    jmethodID getExceptionInfo_;
  };
}

#endif //NATIVE_JNI_SDK_DXTHREADEXCEPTION_HPP
