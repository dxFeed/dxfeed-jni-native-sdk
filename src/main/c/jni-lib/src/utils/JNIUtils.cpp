// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
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
    jmethodID method = (env->*provider)(clazz, methodName, signature);
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

  void registerNativeMethods(JNIEnv* env, jclass clazz) {
    jint methodCount = sizeof(nativeMethods::methods) / sizeof(nativeMethods::methods[0]);
    jint res = env->RegisterNatives(clazz, nativeMethods::methods, methodCount);
    auto msg = (res == JNI_OK) ? "JNI_OK" : "Failed";
    std::cout << "\tRegisterNatives result: " << msg << "(" << res << ")" << std::endl;
  }
}