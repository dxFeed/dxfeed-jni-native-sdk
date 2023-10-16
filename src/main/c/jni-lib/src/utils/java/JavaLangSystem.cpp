// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/JavaLangSystem.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaLangSystem::JavaLangSystem(JNIEnv* env) {
    auto javaLangSystemClazz = safeFindClass(env, "java/lang/System");
    javaLogger->trace(env, "java.lang.System: %", javaLangSystemClazz);
    loadMethodId = safeGetStaticMethodID(env, javaLangSystemClazz, "load", "(Ljava/lang/String;)V");
    javaLogger->trace(env, "void System::load(String path): %", loadMethodId);
    getPropMethodId = safeGetStaticMethodID(env, javaLangSystemClazz, "getProperty",
                                                           "(Ljava/lang/String;)Ljava/lang/String;");
    javaLogger->trace(env, "void System::getProperty(String key): %", getPropMethodId);
    setPropMethodId = safeGetStaticMethodID(env, javaLangSystemClazz, "setProperty",
                                                 "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    javaLogger->trace(env, "String System::setProperty(String key, String value): %", setPropMethodId);
    env->DeleteLocalRef(javaLangSystemClazz);
  }

  const char* JavaLangSystem::getProperty(JNIEnv* env, const char* key) const {
    auto javaLangSystemClazz = safeFindClass(env, "java/lang/System");
    auto jKey = env->NewStringUTF(key);
    auto jValue = checkedCallStaticObjectMethodAndCastTo<jstring>(env, javaLangSystemClazz, getPropMethodId, jKey);
    char* result = new char[1] { 0 };
    if (jValue) {
      auto tmp = env->GetStringUTFChars(jValue, nullptr);
      jsize len = env->GetStringLength(jValue);
      result = new char[len + 1];
      result[len] = 0;
      memcpy(result, tmp, len);
      env->ReleaseStringUTFChars(jValue, nullptr);
      env->DeleteLocalRef(jValue);
    }
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(javaLangSystemClazz);
    return result;
  }

  void JavaLangSystem::setProperty(JNIEnv* env, const char* key, const char* value) {
    auto javaLangSystemClazz = safeFindClass(env, "java/lang/System");
    auto jKey = env->NewStringUTF(key);
    auto jValue = env->NewStringUTF(value);
    checkedCallStaticObjectMethod(env, javaLangSystemClazz, setPropMethodId, jKey, jValue);
    env->DeleteLocalRef(jValue);
    env->DeleteLocalRef(jKey);
    env->DeleteLocalRef(javaLangSystemClazz);
  }
}
