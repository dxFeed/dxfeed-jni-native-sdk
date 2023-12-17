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
      auto errMsg = "Can't find method " + std::string(methodName) + " with signature " + std::string(signature);
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

  jfieldID safeGetFieldID(JNIEnv* env, jclass clazz, const char* fieldName, const char* signature) {
    jfieldID field = env->GetFieldID(clazz, fieldName, signature);
    if (!field) {
      auto errMsg = "Can't find method " + std::string(fieldName) + " with signature " + std::string(signature);
      std::cerr << errMsg << std::endl;
      throw std::runtime_error(errMsg);
    }
    return field;
  }
}