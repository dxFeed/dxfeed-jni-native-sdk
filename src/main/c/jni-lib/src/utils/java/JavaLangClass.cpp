// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaLangClass::JavaLangClass(JNIEnv* env) {
    javaLangClassClazz = safeFindClass(env, "Ljava/lang/Class;");
    javaLogger->info("java.lang.Class: %", javaLangClassClazz);
    getNameMethodId = safeGetMethodID(env, javaLangClassClazz, "getName", "()Ljava/lang/String;");
    javaLogger->info("String Class::getName(): %", getNameMethodId);
    getClassMethodId = safeGetMethodID(env, javaLangClassClazz, "getClass", "()Ljava/lang/Class;");
    javaLogger->info("Class Class::getClass(): %", getClassMethodId);
  }

  std::string JavaLangClass::getName(JNIEnv* env, jobject jObject) const {
    auto clazzObject = env->GetObjectClass(jObject);
    std::string result;
    if (clazzObject) {
      auto name = r_cast<jstring>(env->CallObjectMethod(clazzObject, getNameMethodId));
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
