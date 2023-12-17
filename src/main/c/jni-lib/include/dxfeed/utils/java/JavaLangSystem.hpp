// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_JAVA_LANG_SYSTEM_H_
#define DXFEED_JNI_NATIVE_SDK_JAVA_LANG_SYSTEM_H_

#include <jni.h>

namespace dxfeed::jni {
  struct JavaLangSystem {
    explicit JavaLangSystem(JNIEnv* env);

    const char* getProperty(JNIEnv* env, const char* key) const;
    void setProperty(JNIEnv* env, const char* key, const char* value);

  private:
    jmethodID loadMethodId = nullptr;
    jmethodID getPropMethodId = nullptr;
    jmethodID setPropMethodId = nullptr;
  };

}
#endif // DXFEED_JNI_NATIVE_SDK_JAVA_LANG_SYSTEM_H_