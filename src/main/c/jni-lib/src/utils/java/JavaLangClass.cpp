// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaLangClass::JavaLangClass(JNIEnv* env) {
    auto javaLangClassClazz = safeFindClass(env, "java/lang/Class");
    javaLogger->trace(env, "java.lang.Class: %", javaLangClassClazz);
    getNameMethodId = safeGetMethodID(env, javaLangClassClazz, "getName", "()Ljava/lang/String;");
    javaLogger->trace(env, "String Class::getName(): %", getNameMethodId);
    env->DeleteLocalRef(javaLangClassClazz);
  }

  std::string JavaLangClass::getName(JNIEnv* env, jobject jObject) const {
    auto clazzObject = env->GetObjectClass(jObject);
    return getName(env, clazzObject);
  }

  std::string JavaLangClass::getName(JNIEnv* env, jclass clazzObject) const {
    std::string result;
    if (clazzObject) {
      auto name = checkedCallObjectMethodAndCastTo<jstring>(env, clazzObject, getNameMethodId);
      if (name) {
        const char* data = env->GetStringUTFChars(name, nullptr);
        result = data;
        env->ReleaseStringUTFChars(name, data);
        env->DeleteLocalRef(name);
      }
    }
    env->DeleteLocalRef(clazzObject);
    return result;
  }
}
