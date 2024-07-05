// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_TIME_FORMAT_CLASS_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_TIME_FORMAT_CLASS_H_

#include <jni.h>
#include <string>

namespace dxfeed::jni {
  struct JavaTimeFormat {
    explicit JavaTimeFormat(JNIEnv* env);

//    std::string getName(JNIEnv* env, jobject jObject) const;
  private:
    jmethodID getNameMethodId = nullptr;
  };

}
#endif // DXFEED_JNI_NATIVE_SDK_JAVA_TIME_FORMAT_CLASS_H_