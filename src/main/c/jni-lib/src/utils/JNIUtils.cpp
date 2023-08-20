// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  const char* getJavaHome(VMOptions* params) {
    const char* javaHome = std::getenv(JAVA_HOME);
    const char* msg = "Use JAVA_HOME from ENV: ";
    if (params) {
      auto javaHomeCustom = r_cast<VMOptions*>(params)->javaHome;
      if (javaHomeCustom) {
        msg = "Use custom JAVA_HOME from VmOptions: ";
        javaHome = javaHomeCustom;
      }
    }
    if (!javaHome) {
      throw std::runtime_error("Can't find JAVA_HOME in ENV or in VmOptions!");
    }
    std::cout << msg << javaHome << std::endl;
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