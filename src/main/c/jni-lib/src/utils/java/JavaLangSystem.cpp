// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/JavaLangSystem.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaLangSystem::JavaLangSystem(JNIEnv* env) {
    javaLangSystemClazz = safeFindClass(env, "Ljava/lang/System;");
    javaLogger->info("java.lang.System: %", javaLangSystemClazz);
    loadMethodId = safeGetStaticMethodID(env, javaLangSystemClazz, "load", "(Ljava/lang/String;)V");
    javaLogger->info("void System::load(String path): %", loadMethodId);
    getPropMethodId = safeGetStaticMethodID(env, javaLangSystemClazz, "getProperty",
                                                           "(Ljava/lang/String;)Ljava/lang/String;");
    javaLogger->info("void System::getProperty(String key): %", getPropMethodId);
    setPropMethodId = safeGetStaticMethodID(env, javaLangSystemClazz, "setProperty",
                                                 "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    javaLogger->info("String System::setProperty(String key, String value): %", setPropMethodId);
  }

  void JavaLangSystem::load(JNIEnv* env, const char* libPath) const {
    jstring pStr = env->NewStringUTF(libPath);
    env->CallStaticVoidMethod(javaLangSystemClazz, loadMethodId, pStr);
    env->DeleteLocalRef(pStr);
  }

  const char* JavaLangSystem::getProperty(JNIEnv* env, const char* key) const {
    jstring jKey = env->NewStringUTF(key);
    auto jValue = r_cast<jstring>(env->CallStaticObjectMethod(javaLangSystemClazz, getPropMethodId, jKey));
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
    return result;
  }

  void JavaLangSystem::setProperty(JNIEnv* env, const char* key, const char* value) {
    jstring jKey = env->NewStringUTF(key);
    jstring jValue = env->NewStringUTF(value);
    env->CallStaticObjectMethod(javaLangSystemClazz, setPropMethodId, jKey, jValue);
    env->DeleteLocalRef(jValue);
    env->DeleteLocalRef(jKey);
  }
}
