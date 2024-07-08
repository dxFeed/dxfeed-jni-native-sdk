// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "dxfeed/utils/JNIUtils.hpp"

#if _MSC_VER && !__INTEL_COMPILER
   // for std::getenv
#pragma warning(disable: 4996)

#endif

namespace dxfeed::jni {
  typedef jmethodID (JNIEnv::*JMethodIdProvider)(jclass, const char*, const char*);
  typedef jfieldID (JNIEnv::*JFieldIdProvider)(jclass, const char*, const char*);

  const char* jStringToUTF8(JNIEnv* env, jstring jString) {
    const char* jData = env->GetStringUTFChars(jString, 0);
    const char* copiedData = copy(jData);
    env->ReleaseStringUTFChars(jString, jData);
    return copiedData;
  }

  const char* copy(const char* str) {
    auto len = static_cast<int32_t>(strlen(str));
    char* copiedData = new char[len + 1];
    copiedData[len] = 0;
    memcpy(copiedData, str, len);
    return copiedData;
  }

  const char* getJavaHomeFromEnv() {
    const char* javaHome = std::getenv(JAVA_HOME);
    if (!javaHome) {
      auto errMsg = "Can't find JAVA_HOME in ENV or in VmOptions!";
      std::cerr << errMsg << std::endl;
      throw std::runtime_error(errMsg);
    }
    std::cout << "Use JAVA_HOME from ENV: " << javaHome << std::endl;
    return javaHome;
  }

  jclass safeFindClass(JNIEnv* env, const char* clazzName) {
    auto clazz = env->FindClass(clazzName);
    if (!clazz) {
      auto errMsg = "Can't find class in classpath: " + std::string(clazzName);
      std::cerr << errMsg << std::endl;
      throw std::runtime_error(errMsg);
    }
    return clazz;
  }

  jmethodID safeGetMethod(JNIEnv* env, JMethodIdProvider provider, jclass clazz, const char* methodName,
                          const char* signature)
  {
    auto method = (env->*provider)(clazz, methodName, signature);
    if (!method) {
      auto className = dxfeed::jni::internal::javaLangClass->getName(env, clazz);
      auto errMsg = "Can't find method " + std::string(methodName) + " with signature " + std::string(signature)
          + " in class: " + className;
      std::cerr << errMsg << std::endl;
      throw std::runtime_error(errMsg);
    }
    return method;
  }

  jmethodID safeGetStaticMethodID(JNIEnv* env, jclass clazz, const char* methodName, const char* signature) {
    return safeGetMethod(env, &JNIEnv::GetStaticMethodID, clazz, methodName, signature);
  }

  jmethodID safeGetMethodID(JNIEnv* env, jclass clazz, const char* methodName, const char* signature) {
    return safeGetMethod(env, &JNIEnv::GetMethodID, clazz, methodName, signature);
  }

  jfieldID safeGetField(JNIEnv* env, JFieldIdProvider provider, jclass clazz, const char* fieldName,
                          const char* signature)
  {
    auto field = (env->*provider)(clazz, fieldName, signature);
    if (!field) {
      auto errMsg = "Can't find field " + std::string(fieldName) + " with signature " + std::string(signature);
      std::cerr << errMsg << std::endl;
      throw std::runtime_error(errMsg);
    }
    return field;
  }

  jfieldID safeGetStaticFieldID(JNIEnv* env, jclass clazz, const char* fieldName, const char* signature) {
    return safeGetField(env, &JNIEnv::GetStaticFieldID, clazz, fieldName, signature);
  }

  jfieldID safeGetFieldID(JNIEnv* env, jclass clazz, const char* fieldName, const char* signature) {
    return safeGetField(env, &JNIEnv::GetFieldID, clazz, fieldName, signature);
  }
}