// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/utils/java/JavaTimeFormat.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaTimeFormat::JavaTimeFormat(JNIEnv* env) {
    auto javaTimeFormatClazz = safeFindClass(env, "com/devexperts/util/TimeFormat");
    javaLogger->trace(env, "com.devexperts.util.TimeFormat: %", javaTimeFormatClazz);
//    getNameMethodId = safeGetMethodID(env, javaTimeFormatClazz, "getName", "()Ljava/lang/String;");
//    javaLogger->trace(env, "String Class::getName(): %", getNameMethodId);
    env->DeleteLocalRef(javaTimeFormatClazz);
  }

//  std::string JavaLangClass::getName(JNIEnv* env, jobject jObject) const {
//    auto clazzObject = env->GetObjectClass(jObject);
//    std::string result;
//    if (clazzObject) {
//      auto name = checkedCallObjectMethodAndCastTo<jstring>(env, clazzObject, getNameMethodId);
//      if (name) {
//        const char* data = env->GetStringUTFChars(name, nullptr);
//        result = data;
//        env->ReleaseStringUTFChars(name, data);
//        env->DeleteLocalRef(name);
//      }
//    }
//    env->DeleteLocalRef(clazzObject);
//    return result;
//  }
}
