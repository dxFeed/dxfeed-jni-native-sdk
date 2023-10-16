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
  JavaLangSystem* javaLangSystem = nullptr;
  DxThreadException* dxThreadException = nullptr;
  const JavaLangClass* javaLangClass = nullptr;

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

    javaLogger->trace(env, "JAVA_HOME info:");
    javaLogger->trace(env, "\t % version \"%\" %", *vendor, *version, *versionDate);
    javaLogger->trace(env, "\t %, (build %)", *runtimeName, *runtimeVersion);
    javaLogger->trace(env, "\t % %, (build %, %)", *vmName, *vmVendor, *vmVersion, *vmInfo);
  }

  void loadLibrary(JNIEnv* env, const char* libPath) {
    const auto jDxFeedJniClazz = safeFindClass(env, "com/dxfeed/api/DxFeedJni");
    auto loadMethodId = safeGetStaticMethodID(env, jDxFeedJniClazz, "loadLibrary", "(Ljava/lang/String;)V");
    auto pStr = env->NewStringUTF(libPath);
    checkedCallStaticVoidMethod(env, jDxFeedJniClazz, loadMethodId, pStr);
    env->DeleteLocalRef(pStr);
    env->DeleteLocalRef(jDxFeedJniClazz);
  }

  void loadJNILibrary(JNIEnv* env) {
    dxThreadException = new DxThreadException(env);
    javaLangClass = new JavaLangClass(env);
    javaLangSystem = new JavaLangSystem(env);
    loadLibrary(env, dllFilePath);
    javaLogger->trace(env, "Loaded DxFeed lib: %", dllFilePath);
    auto property = std::make_unique<const char*>(
      javaLangSystem->getProperty(env, "com.devexperts.qd.impl.matrix.Agent.MaxBufferSize"));
    javaLogger->trace(env, " com.devexperts.qd.impl.matrix.Agent.MaxBufferSize = %", *property);
    dumpJavaInfo(env);
    initDxJni(env);
  }

  std::string buildClassPath(const fs::path& runtimePath) {
    fs::path jarPath = runtimePath / MY_JAR;
    if (!exists(jarPath)) {
      throw std::runtime_error("Can't find java libs in " + jarPath.string());
    }
    std::cout << "DxFeed JAR path: " << jarPath << std::endl;
    return "-Djava.class.path=" + jarPath.string();
  }

  void dumpJavaCmd(const JavaVMOption* javaVmOptions, int vmOptionsCount) {
    std::cout << "Run cmd \"java";
    for (int i = 0; i < vmOptionsCount; ++i) {
      std::cout << " " << javaVmOptions[i].optionString;
    }
    std::cout << "\"" << std::endl;
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

    dumpJavaCmd(javaVmOptions, vmOptionsCount);

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
    javaLogger = new JavaLogger(jniEnv);

    loadJNILibrary(jniEnv);
  }
} // end of namespace dxfeed::jni::internal
} // end of namespace dxfeed::jni