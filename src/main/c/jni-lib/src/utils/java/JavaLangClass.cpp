// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include "dxfeed/utils/java/JavaLangClass.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  JavaLangClass::JavaLangClass(JNIEnv* env) {
    javaLangClassClazz = jni::safeFindClass(env, "Ljava/lang/Class;");
    std::cout << "java.lang.Class: " << javaLangClassClazz << "\n";
    getNameMethodId = jni::safeGetMethodID(env, javaLangClassClazz, "getName", "()Ljava/lang/String;");
    std::cout << "String Class::getName(): " << getNameMethodId << "\n";
    getClassMethodId = jni::safeGetMethodID(env, javaLangClassClazz, "getClass", "()Ljava/lang/Class;");
    std::cout << "Class Class::getClass(): " << getClassMethodId << "\n";
  }

  std::string JavaLangClass::getName(JNIEnv* env, jobject jObject) const {
    auto clazzObject = env->GetObjectClass(jObject);
    std::string result;
    if (clazzObject) {
      auto name = reinterpret_cast<jstring>(env->CallObjectMethod(clazzObject, getNameMethodId));
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
