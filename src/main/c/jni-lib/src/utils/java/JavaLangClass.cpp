// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaLangClass::JavaLangClass(JNIEnv* env) {
    auto javaLangClassClazz = safeFindClass(env, "java/lang/Class");
    javaLogger->info(env, "java.lang.Class: %", javaLangClassClazz);
    getNameMethodId = safeGetMethodID(env, javaLangClassClazz, "getName", "()Ljava/lang/String;");
    javaLogger->info(env, "String Class::getName(): %", getNameMethodId);
    env->DeleteLocalRef(javaLangClassClazz);
  }

  std::string JavaLangClass::getName(JNIEnv* env, jobject jObject) const {
    auto clazzObject = env->GetObjectClass(jObject);
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
