// SPDX-License-Identifier: MPL-2.0

#include <stdexcept>
#include <iostream>

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/Connection.hpp"
#include "dxfeed/utils/LoadLibrary.hpp"

namespace dxfeed {
  void dxfeed::DxFeed::initJavaVM(const char* javaHome, const char** consoleVmArgs, const int vmArgsCount) {
    dxfeed::internal::loadJavaVM(javaHome, consoleVmArgs, vmArgsCount);
  }

  DxFeed& dxfeed::DxFeed::getInstance() {
    static DxFeed instance;
    return instance;
  }

  DxFeed::DxFeed() :
    env_{jniEnv}
 {
   javaHelperClass_ = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JniTest;");
   addEventListenerHelperMethodId_ = jni::safeGetStaticMethodID(jniEnv, javaHelperClass_, "addEventListener",
                                                                "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
 }

  jclass DxFeed::helperClass() {
    return javaHelperClass_;
  }

  jmethodID DxFeed::addEventListenerMethod() {
    return addEventListenerHelperMethodId_;
  }

  void DxFeed::onClose(jobject clazz) {
    jniEnv->DeleteGlobalRef(clazz);
  }

  Connection* DxFeed::createConnection(const std::string& address) {
    return new Connection(env_, address, onClose);
  }

  jclass jni::safeFindClass(JNIEnv* env, const char* clazzName) {
    auto clazz = env->FindClass(clazzName);
    if (!clazz) {
      auto errMsg = "Can't find class in classpath: " + std::string(clazzName);
      std::cerr << errMsg << std::endl;
      throw std::runtime_error(errMsg);
    }
    return clazz;
  }

  jmethodID jni::safeGetMethod(JNIEnv* env, JMethodIdProvider provider, jclass clazz, const char* methodName,
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

  jmethodID jni::safeGetStaticMethodID(JNIEnv* env, jclass clazz, const char* methodName, const char* signature) {
    return safeGetMethod(env, &JNIEnv::GetStaticMethodID, clazz, methodName, signature);
  }

  jmethodID jni::safeGetMethodID(JNIEnv* env, jclass clazz, const char* methodName, const char* signature) {
    return safeGetMethod(env, &JNIEnv::GetMethodID, clazz, methodName, signature);
  }
}
