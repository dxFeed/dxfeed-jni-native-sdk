// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/vm/JavaVmInstance.hpp"

namespace dxfeed::jni::internal::vm {
  JavaVmInstance::JavaVmInstance(JavaVM* vmPtr, const int jniVersion) :
      vm_(vmPtr),
      jniVersion_(jniVersion),
      criticalSection_()
  {}

  JavaVmInstance::~JavaVmInstance() {
    vm_->DestroyJavaVM();
    vm_ = nullptr;
  }

  JNIEnv* JavaVmInstance::getCurrenThread() {
    criticalSection_.enter();
    JNIEnv* env = nullptr;
    jint hr = vm_->GetEnv((void**)&env, jniVersion_);
    if (hr == JNI_ERR) {
      std::cerr << "Error getting JNIEnv. Exiting..." << std::endl;
      return nullptr;
    }
    std::cout << "getCurrenThread, env = " << env << std::endl;
    criticalSection_.leave();
    return env;
  }

  bool JavaVmInstance::attachCurrentThread(JNIEnv** env) {
    criticalSection_.enter();
    jint hr = vm_->AttachCurrentThread((void**) env, nullptr);
    bool success = (hr == JNI_OK);
    if (success) {
      std::cout << "New thread is attached. tid: " /** PID */ << "\n"; // todo: getPid cross-platform
    } else {
      logHRESULT(*env, hr);
    }
    criticalSection_.leave();
    return success;
  }

  void JavaVmInstance::detachCurrentThread() {
    criticalSection_.enter();
    static thread_local DetachJniThreadOnExit onExit { vm_ };
    criticalSection_.leave();
  }

  void JavaVmInstance::logErrMsg(JNIEnv* env, jint hr, const char* errMsg) {
    std::cerr << "ENV = " << env << ", hr = " << hrToMsg(hr) << ", err = " << errMsg;
  }

  void JavaVmInstance::logHRESULT(JNIEnv* env, jint hr) {
    switch (hr) {
      case JNI_ERR: {
        logErrMsg(env, hr, "unknown error");
      }
      case JNI_EDETACHED: {
        logErrMsg(env, hr, "thread detached from the VM. JNIEnv* env is nullptr");
      }
      case JNI_EVERSION: {
        logErrMsg(env, hr, "the specified JNI version is not supported");
      }
      case JNI_ENOMEM: {
        logErrMsg(env, hr, "not enough memory");
      }
      case JNI_EEXIST: {
        logErrMsg(env, hr, "VM is already created");
      }
      case JNI_EINVAL: {
        logErrMsg(env, hr, "invalid arguments");
      }
      default: {
        logErrMsg(env, hr, "unknown error");
      }
    }
  }

  const char* JavaVmInstance::hrToMsg(int hr) {
    switch (hr) {
      case JNI_ERR: {
        return "JNI_ERR(-1)";
      }
      case JNI_EDETACHED: {
        return "JNI_EDETACHED(-2)";
      }
      case JNI_EVERSION: {
        return "JNI_EVERSION(-3)";
      }
      case JNI_ENOMEM: {
        return "JNI_ENOMEM(-4)";
      }
      case JNI_EEXIST: {
        return "JNI_EEXIST(-5)";
      }
      case JNI_EINVAL: {
        return "JNI_EINVAL(-6)";
      }
      default: {
        return "UNKNOWN_JNI_ERROR";
      }
    }
  }

  JavaVmInstance::DetachJniThreadOnExit::DetachJniThreadOnExit(JavaVM* vmPtr) : vmCached_(vmPtr) {}

  JavaVmInstance::DetachJniThreadOnExit::~DetachJniThreadOnExit() {
    std::cout << "Detaching thread tid: " /** PID */ << "\n"; // todo: getPid cross-platform
    jint hr = vmCached_->DetachCurrentThread();
    if (hr != JNI_OK) {
      std::cerr << "hr = " << hrToMsg(hr);
    }
  }
}
