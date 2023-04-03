#pragma once

#include <filesystem>
#include <jni.h>

namespace fs = std::filesystem;
namespace dxfeed::internal {

  typedef jint(JNICALL* CreateJavaVM_t)(JavaVM** pvm, void** env, void* args);
  typedef jclass(JNICALL* FindClassFromBootLoader_t)(JNIEnv* env, const char* name);

  extern CreateJavaVM_t createJavaVM;

  void loadJVMLibrary(const char*);
  void loadJavaVM(const char* javaHome,  char** argv = nullptr, int vmArgsCount = 0);
}