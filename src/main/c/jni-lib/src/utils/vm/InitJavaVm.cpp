// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  const JavaLogger* javaLogger = nullptr;

namespace internal {
  extern char dllFilePath[];
  const char MY_JAR[] = "dxfeed-jni-native-sdk-0.1.0.jar";

  void loadJVMLibrary(const char*);

  JNIEnv* jniEnv = nullptr;
  JVMInstance* javaVM = nullptr;
  const JavaLangSystem* javaLangSystem = nullptr;
  const JavaLangClass* javaLangClass = nullptr;
  const DxJni* dxJni = nullptr;


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
    auto vendor = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.vendor"));
    auto version = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.version"));
    auto versionDate = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.version.date"));
    auto runtimeName = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.runtime.name"));
    auto runtimeVersion = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.runtime.version"));
    auto vmName = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.vm.name"));
    auto vmVendor = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.vm.vendor"));
    auto vmVersion = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.vm.version"));
    auto vmInfo = std::make_unique <const char*>(javaLangSystem->getProperty(env, "java.vm.info"));

    javaLogger->info("JAVA_HOME info:");
    javaLogger->info("\t % version \"%\" %", *vendor, *version, *versionDate);
    javaLogger->info("\t %, (build %)", *runtimeName, *runtimeVersion);
    javaLogger->info("\t % %, (build %, %)", *vmName, *vmVendor, *vmVersion, *vmInfo);
  }

  void loadJNILibrary(JNIEnv* env) {
    javaLangClass = new JavaLangClass(env);
    javaLangSystem = new JavaLangSystem(env);
    javaLangSystem->load(env, dllFilePath);
    javaLogger->info("Loaded DxFeed lib: %", dllFilePath);
    dumpJavaInfo(env);
    dxJni = DxJni::initDxJni(env);
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
    auto javaVmOptionsPtr = std::make_unique <JavaVMOption[]>(vmOptionsCount);
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
    javaLogger = new JavaLogger(jniEnv);
    javaVM = new vm::JavaVmInstance(javaVmPtr, vmArgs.version);

    loadJNILibrary(jniEnv);
  }
} // end of namespace dxfeed::jni::internal
} // end of namespace dxfeed::jni