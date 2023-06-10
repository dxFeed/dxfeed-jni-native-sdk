// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

#include "dxfeed/utils/JNIUtils.hpp"
#include "javah/com_dxfeed_api_JNIDXFeedEventListener.h"
#include "javah/com_dxfeed_api_JNIPropertyChangeListener.h"

namespace dxfeed::jni::internal {
  extern char dllFilePath[];
  const char MY_JAR[] = "dxfeed-jni-native-sdk-0.1.0.jar";

  void loadJVMLibrary(const char*);

  JNIEnv* jniEnv = nullptr;
  JVMInstance* javaVM = nullptr;
  const JavaLangSystem* javaLangSystem = nullptr;
  const JavaLangClass* javaLangClass = nullptr;

  namespace nativeMethods {
    static JNINativeMethod dxFeedEventListenerMethods[] = {
        {"nOnEventListener", "(I[B[D[BJJ)V", (void*) &Java_com_dxfeed_api_JNIDXFeedEventListener_nOnEventListener}
    };

    int32_t dxFeedEventListenerMethodsMethodsCount() {
      return sizeof(dxFeedEventListenerMethods) / sizeof(dxFeedEventListenerMethods[0]);
    }

    static JNINativeMethod propertyStateChangeListenerMethods[] = {
        {"nOnStateChangeListener", "(IIJJ)V", (void*)
        &Java_com_dxfeed_api_JNIPropertyChangeListener_nOnStateChangeListener}
    };

    int32_t propertyStateChangeListenerMethodsCount() {
      return sizeof(propertyStateChangeListenerMethods) / sizeof(propertyStateChangeListenerMethods[0]);
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

  void dumpJavaInfo() {
    auto vendor = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.vendor"));
    auto version = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.version"));
    auto versionDate = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.version.date"));
    auto runtimeName = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.runtime.name"));
    auto runtimeVersion = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.runtime.version"));
    auto vmName = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.vm.name"));
    auto vmVendor = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.vm.vendor"));
    auto vmVersion = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.vm.version"));
    auto vmInfo = std::make_unique<const char*>(javaLangSystem->getProperty(jniEnv, "java.vm.info"));

    std::cout << "JAVA_HOME info:" << std::endl;
    std::cout << "\t" << *vendor << " version \"" << *version << "\" " << *versionDate << std::endl;
    std::cout << "\t" << *runtimeName << " (build " << *runtimeVersion << ")" << std::endl;
    std::cout << "\t" << *vmName << " " << *vmVendor << " (build" << *vmVersion << ", " << *vmInfo << ")" << std::endl;
  }

  void registerNativeMethods(JNIEnv* env, jclass clazz, JNINativeMethod* methods, int methodCount) {
    jint res = env->RegisterNatives(clazz, methods, methodCount);
    auto msg = (res == JNI_OK) ? "JNI_OK" : "Failed";
    std::cout << "\tRegisterNatives result: " << msg << "(" << res << ")" << std::endl;
  }

  void loadJNILibrary() {
    javaLangClass = new JavaLangClass(jniEnv);
    javaLangSystem = new JavaLangSystem(jniEnv);
    javaLangSystem->load(jniEnv, dllFilePath);
    std::cout << "Loaded DxFeed lib: " << dllFilePath << std::endl;
    dumpJavaInfo();

    jclass dxFeedEventListenerClazz = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JNIDXFeedEventListener;");
    std::cout << "\tclazz com/dxfeed/api/JNIDXFeedEventListener: " << dxFeedEventListenerClazz << std::endl;
    registerNativeMethods(jniEnv, dxFeedEventListenerClazz,
                          nativeMethods::dxFeedEventListenerMethods,
                          nativeMethods::dxFeedEventListenerMethodsMethodsCount());
    jniEnv->DeleteLocalRef(dxFeedEventListenerClazz);

    jclass propertyChangeListenerClazz = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JNIPropertyChangeListener;");
    std::cout << "\tclazz com/dxfeed/api/JNIPropertyChangeListener: " << propertyChangeListenerClazz << std::endl;
    registerNativeMethods(jniEnv, propertyChangeListenerClazz,
                          nativeMethods::propertyStateChangeListenerMethods,
                          nativeMethods::propertyStateChangeListenerMethodsCount());
    jniEnv->DeleteLocalRef(propertyChangeListenerClazz);
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
    javaVM = new vm::JavaVmInstance(javaVmPtr, vmArgs.version);

    loadJNILibrary();
  }
}