// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#include "dxfeed/utils/JNIUtils.hpp"
#include "javah/com_dxfeed_api_DxFeedJni.h"

namespace dxfeed::jni::internal {
  extern char dllFilePath[];
  const char MY_JAR[] = "dxfeed-jni-native-sdk-0.1.0.jar";

  void loadJVMLibrary(const char*);

  JNIEnv* jniEnv = nullptr;
  JVMInstance* javaVM = nullptr;
  const JavaLangSystem* javaLangSystem = nullptr;
  const JavaLangClass* javaLangClass = nullptr;

  namespace nativeMethods {
    static JNINativeMethod nativeMethods[] = {
        {"nOnStateChangeListener", "(IIJJ)V", (void*) &Java_com_dxfeed_api_DxFeedJni_nOnStateChangeListener},
        {"nOnEventListener", "(I[B[D[BJJ)V", (void*) &Java_com_dxfeed_api_DxFeedJni_nOnEventListener}
    };

    void registerNativeMethods(JNIEnv* env) {
      jclass dxFeedJniClazz = jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
      std::cout << "\tclazz com/dxfeed/api/DxFeedJni: " << dxFeedJniClazz << std::endl;
      jint res = env->RegisterNatives(dxFeedJniClazz, nativeMethods, sizeof(nativeMethods) / sizeof(nativeMethods[0]));
      env->DeleteLocalRef(dxFeedJniClazz);
      auto msg = (res == JNI_OK) ? "JNI_OK" : "Failed";
      std::cout << "\tRegisterNatives result: " << msg << "(" << res << ")" << std::endl;
    }
  }

  void addJavaVMArgs(JavaVMOption* vmOptions, const char* vmArgs[], int vmArgCount) {
    if (vmArgs) {
      for (int i = 0; i < vmArgCount; ++i) {
        auto arg = vmArgs[i];
        if (arg[0] == '-') { // VM args starts from '-'
          vmOptions[i].optionString = const_cast<char*>(arg);
        } else {
          std::cout << "unknown VM arg: " << arg << std::endl;
        }
      }
    }
  }

  void dumpJavaInfo(JNIEnv* env) {
    auto vendor = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.vendor"));
    auto version = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.version"));
    auto versionDate = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.version.date"));
    auto runtimeName = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.runtime.name"));
    auto runtimeVersion = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.runtime.version"));
    auto vmName = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.vm.name"));
    auto vmVendor = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.vm.vendor"));
    auto vmVersion = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.vm.version"));
    auto vmInfo = std::make_unique<const char*>(javaLangSystem->getProperty(env, "java.vm.info"));

    std::cout << "JAVA_HOME info:" << std::endl;
    std::cout << "\t" << *vendor << " version \"" << *version << "\" " << *versionDate << std::endl;
    std::cout << "\t" << *runtimeName << " (build " << *runtimeVersion << ")" << std::endl;
    std::cout << "\t" << *vmName << " " << *vmVendor << " (build" << *vmVersion << ", " << *vmInfo << ")" << std::endl;
  }

  void loadJNILibrary(JNIEnv* env) {
    javaLangClass = new JavaLangClass(env);
    javaLangSystem = new JavaLangSystem(env);
    javaLangSystem->load(env, dllFilePath);
    std::cout << "Loaded DxFeed lib: " << dllFilePath << std::endl;
    dumpJavaInfo(env);
    nativeMethods::registerNativeMethods(env);
  }

  std::string buildClassPath(const fs::path& runtimePath) {
    fs::path jarPath = runtimePath / MY_JAR;
    if (!exists(jarPath)) {
      throw std::runtime_error("Can't find java libs in " + jarPath.string());
    }
    std::cout << "DxFeed JAR path: " << jarPath << std::endl;
    return "-Djava.class.path=" + jarPath.string();
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

    std::cout << "Run cmd \"java " << javaVmOptions->optionString << "\"" << std::endl;

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
    javaVM = vm::JavaVmInstance::getInstance(javaVmPtr, vmArgs.version);

    loadJNILibrary(jniEnv);
  }
}