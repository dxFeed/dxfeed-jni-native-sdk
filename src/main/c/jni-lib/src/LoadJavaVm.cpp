// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <string>
#include <iostream>

#include "dxfeed/utils/LoadLibrary.hpp"
#include "dxfeed/utils/JavaProperty.hpp"

namespace dxfeed {
  JNIEnv* jniEnv = nullptr;
  JavaVM* javaVM = nullptr;

  namespace internal {
#if _MSC_VER && !__INTEL_COMPILER
    constexpr char JAR_SEPARATOR = ';';
    const char PATH_SEPARATOR = '\\';
    const char JNI_LIB_NAME[] = "native_jni.dll";
#else
    constexpr char JAR_SEPARATOR = ':';
    constexpr char PATH_SEPARATOR = '/';
    const char JNI_LIB_NAME[] = "native_jni.dylib";
#endif
    const char MY_JAR[] = "dxfeed-jni-native-sdk-0.0.1-SNAPSHOT.jar";
    const char* JARS[] = {
      "auther-api-442.jar",
      "dxfeed-api-3.313.jar",
      "dxlib-3.313.jar",
      "qds-3.313.jar",
      "qds-file-3.313.jar",
      "qds-tools-3.313.jar"
    };

    std::string buildClassPath(const std::filesystem::path& runtimePath) {
      auto jarPath = std::filesystem::absolute(runtimePath).append("java-libs");
      if (!exists(jarPath)) {
        throw std::runtime_error("Can't find java libs in " + jarPath.string());
      }
      auto jarDxFeedPath = absolute(jarPath).append("libs").string() + PATH_SEPARATOR;
      std::cout << "Custom JAR path: " <<  jarPath << std::endl;
      std::cout << "DxFeed JAR path: " <<  jarDxFeedPath << std::endl;

      std::filesystem::path result = jarPath.string() + PATH_SEPARATOR + MY_JAR;
      for (const auto jar : JARS) {
        result +=  JAR_SEPARATOR + jarDxFeedPath +  jar;
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
      auto vendor = JavaProperty { pJclass, methodId, "java.vendor"};
      auto version = JavaProperty { pJclass, methodId, "java.version"};
      auto versionDate = JavaProperty { pJclass, methodId, "java.version.date"};
      auto runtimeName = JavaProperty { pJclass, methodId, "java.runtime.name"};
      auto runtimeVersion = JavaProperty { pJclass, methodId, "java.runtime.version"};
      auto vmName = JavaProperty { pJclass, methodId, "java.vm.name"};
      auto vmVendor = JavaProperty { pJclass, methodId, "java.vm.vendor"};
      auto vmVersion = JavaProperty { pJclass, methodId, "java.vm.version"};
      auto vmInfo = JavaProperty { pJclass, methodId, "java.vm.info"};

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

      jmethodID getPropMethodId = jni::safeGetStaticMethodID(jniEnv, javaLangSystemClazz, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
      std::cout << "void System::getProperty(String path): " << getPropMethodId << "\n";
      dumpJavaInfo(javaLangSystemClazz, getPropMethodId);

      jclass clazz = jni::safeFindClass(jniEnv, "com/dxfeed/api/JniTest");
      std::cout << "\tclazz com/dxfeed/api/JniTest: " << clazz << std::endl;

      jint res = jniEnv->RegisterNatives(clazz, methods, sizeof(methods)/sizeof(methods[0]));
      auto msg = (res == JNI_OK) ? "JNI_OK" : "Failed";
      std::cout << "\tRegisterNatives result: " << msg << "(" << res << ")" << std::endl;
    }

    void loadJavaVM(const char* javaHome, const char** consoleVmArgs, const int vmArgsCount) {
      internal::loadJVMLibrary(javaHome);

      auto runtimePath = fs::current_path();
      std::cout << "APP_RUNTIME_PATH: " <<  runtimePath << std::endl;

      int vmOptionsCount = vmArgsCount + 1; // 1 for classpath
      auto javaVmOptionsPtr = std::make_unique<JavaVMOption[]>(vmOptionsCount);
      auto javaVmOptions = javaVmOptionsPtr.get();
      std::string classPath = buildClassPath(runtimePath);
      javaVmOptions[0].optionString = classPath.data();
      addJavaVMArgs(javaVmOptions + 1, consoleVmArgs, vmArgsCount);

      JavaVMInitArgs vmArgs;
      vmArgs.version = JNI_VERSION_1_8;
      vmArgs.options = javaVmOptions;
      vmArgs.nOptions = vmOptionsCount;
      vmArgs.ignoreUnrecognized = JNI_FALSE;

      // Create the JVM
      jint flag = internal::createJavaVM(&javaVM, (void**) &jniEnv, &vmArgs);
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
}