// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <string>
#include <iostream>

#include "dxfeed/utils/InitJavaVm.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/JavaProperty.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace dxfeed::jni::internal {
  JNIEnv* jniEnv = nullptr;
  JavaVM* javaVM = nullptr;

  std::string buildClassPath(const fs::path& runtimePath) {
    auto jarPath = fs::absolute(runtimePath).append("java-libs");
    if (!exists(jarPath)) {
      throw std::runtime_error("Can't find java libs in " + jarPath.string());
    }
    auto jarDxFeedPath = absolute(jarPath).append("libs").string() + PATH_SEPARATOR;
    std::cout << "Custom JAR path: " << jarPath << std::endl;
    std::cout << "DxFeed JAR path: " << jarDxFeedPath << std::endl;

    fs::path result = jarPath.string() + PATH_SEPARATOR + MY_JAR;
    for (const auto jar: JARS) {
      result += JAR_SEPARATOR + jarDxFeedPath + jar;
    }
    return "-Djava.class.path=" + result.string();
  }

  void addJavaVMArgs(JavaVMOption* vmOptions, const char* vmArgs[], int vmArgCount) {
    for (int i = 0; i < vmArgCount; ++i) {
      auto arg = vmArgs[i];
      if (arg[0] == '-') { // VM args starts from '-'
        vmOptions[i].optionString = const_cast<char*>(arg);
      } else {
        std::cout << "unknown VM arg: " << arg << std::endl;
      }
    }
  }

  void dumpJavaInfo(jclass pJclass, jmethodID methodId) {
    auto vendor = JavaProperty{jniEnv, pJclass, methodId, "java.vendor"};
    auto version = JavaProperty{jniEnv, pJclass, methodId, "java.version"};
    auto versionDate = JavaProperty{jniEnv, pJclass, methodId, "java.version.date"};
    auto runtimeName = JavaProperty{jniEnv, pJclass, methodId, "java.runtime.name"};
    auto runtimeVersion = JavaProperty{jniEnv, pJclass, methodId, "java.runtime.version"};
    auto vmName = JavaProperty{jniEnv, pJclass, methodId, "java.vm.name"};
    auto vmVendor = JavaProperty{jniEnv, pJclass, methodId, "java.vm.vendor"};
    auto vmVersion = JavaProperty{jniEnv, pJclass, methodId, "java.vm.version"};
    auto vmInfo = JavaProperty{jniEnv, pJclass, methodId, "java.vm.info"};

    std::cout << "JAVA_HOME info:" << std::endl;
    std::cout << "\t" << vendor << " version \"" << version << "\" " << versionDate << std::endl;
    std::cout << "\t" << runtimeName << " (build " << runtimeVersion << ")" << std::endl;
    std::cout << "\t" << vmName << " " << vmVendor << " (build" << vmVersion << ", " << vmInfo << ")" << std::endl;
  }

  void loadJNILibrary(const char* path) {
    jclass javaLangSystemClazz = jni::safeFindClass(jniEnv, "Ljava/lang/System;");
    std::cout << "java.lang.System: " << javaLangSystemClazz << "\n";
    jmethodID loadMethodId = jni::safeGetStaticMethodID(jniEnv, javaLangSystemClazz, "load", "(Ljava/lang/String;)V");
    std::cout << "void System::load(String path): " << loadMethodId << "\n";
    jstring pJstring = jniEnv->NewStringUTF(path);
    jniEnv->CallStaticVoidMethod(javaLangSystemClazz, loadMethodId, pJstring);

    jmethodID getPropMethodId = jni::safeGetStaticMethodID(jniEnv, javaLangSystemClazz, "getProperty",
                                                           "(Ljava/lang/String;)Ljava/lang/String;");
    std::cout << "void System::getProperty(String path): " << getPropMethodId << "\n";
    dumpJavaInfo(javaLangSystemClazz, getPropMethodId);

    jclass clazz = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JniTest;");
    std::cout << "\tclazz com/dxfeed/api/JniTest: " << clazz << std::endl;
    jni::registerNativeMethods(jniEnv, clazz);

    jniEnv->DeleteLocalRef(pJstring);
    jniEnv->DeleteLocalRef(javaLangSystemClazz);
    jniEnv->DeleteLocalRef(clazz);
  }

  void initJavaVM(VMOptions* params) {
    loadJVMLibrary(params->javaHome);

    auto runtimePath = fs::current_path();
    std::cout << "APP_RUNTIME_PATH: " << runtimePath << std::endl;

    int vmOptionsCount = params->vmArgsCount + 1; // 1 for classpath
    auto javaVmOptionsPtr = std::make_unique<JavaVMOption[]>(vmOptionsCount);
    auto javaVmOptions = javaVmOptionsPtr.get();
    std::string classPath = buildClassPath(runtimePath);
    javaVmOptions[0].optionString = classPath.data();
    addJavaVMArgs(javaVmOptions + 1, params->vmArgs, params->vmArgsCount);

    JavaVMInitArgs vmArgs;
    vmArgs.version = JNI_VERSION_1_8;
    vmArgs.options = javaVmOptions;
    vmArgs.nOptions = vmOptionsCount;
    vmArgs.ignoreUnrecognized = JNI_FALSE;

    // Create the JVM
    jint flag = fCreateJavaVM(&javaVM, (void**) &jniEnv, &vmArgs);
    if (flag == JNI_ERR) {
      throw std::runtime_error("Error creating VM. Exiting...n");
    }

    auto path = runtimePath.string() + PATH_SEPARATOR + JNI_LIB_NAME;
    loadJNILibrary(path.c_str());
  }

  // todo: add dispose!!!!!!!!!!!!!
  void disposeJavaVM() {
    javaVM->DestroyJavaVM();
  }
}