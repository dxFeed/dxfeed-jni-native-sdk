// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <string>
#include <iostream>

#include "dxfeed/utils/InitJavaVm.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace dxfeed::jni::internal {
  JNIEnv* jniEnv = nullptr;
  JVMInstance* javaVM = nullptr;
  const JavaLangSystem* javaLangSystem = nullptr;

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

  void dumpJavaInfo() {
    auto vendor = javaLangSystem->getProperty(jniEnv, "java.vendor");
    auto version = javaLangSystem->getProperty(jniEnv, "java.version");
    auto versionDate = javaLangSystem->getProperty(jniEnv, "java.version.date");
    auto runtimeName = javaLangSystem->getProperty(jniEnv, "java.runtime.name");
    auto runtimeVersion = javaLangSystem->getProperty(jniEnv, "java.runtime.version");
    auto vmName = javaLangSystem->getProperty(jniEnv, "java.vm.name");
    auto vmVendor = javaLangSystem->getProperty(jniEnv, "java.vm.vendor");
    auto vmVersion = javaLangSystem->getProperty(jniEnv, "java.vm.version");
    auto vmInfo = javaLangSystem->getProperty(jniEnv, "java.vm.info");

    std::cout << "JAVA_HOME info:" << std::endl;
    std::cout << "\t" << vendor << " version \"" << version << "\" " << versionDate << std::endl;
    std::cout << "\t" << runtimeName << " (build " << runtimeVersion << ")" << std::endl;
    std::cout << "\t" << vmName << " " << vmVendor << " (build" << vmVersion << ", " << vmInfo << ")" << std::endl;

    delete[] vmInfo;
    delete[] vmVersion;
    delete[] vmVendor;
    delete[] vmName;
    delete[] runtimeVersion;
    delete[] runtimeName;
    delete[] versionDate;
    delete[] version;
    delete[] vendor;
  }

  void loadJNILibrary(const char* path) {
    javaLangSystem = new JavaLangSystem(jniEnv);
    javaLangSystem->load(jniEnv, path);
    dumpJavaInfo();

    jclass clazz = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JniTest;");
    std::cout << "\tclazz com/dxfeed/api/JniTest: " << clazz << std::endl;
    jni::registerNativeMethods(jniEnv, clazz);
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
    JavaVM* javaVmPtr;
    jint flag = fCreateJavaVM(&javaVmPtr, (void**) &jniEnv, &vmArgs);
    if (flag == JNI_ERR) {
      throw std::runtime_error("Error creating VM. Exiting...n");
    }
    javaVM = new vm::JavaVmInstance(javaVmPtr, vmArgs.version);

    auto path = runtimePath.string() + PATH_SEPARATOR + JNI_LIB_NAME;
    loadJNILibrary(path.c_str());
  }
}