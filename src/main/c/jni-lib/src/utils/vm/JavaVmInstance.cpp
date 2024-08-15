// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/vm/JavaVmInstance.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed::jni::internal::vm {
JavaVmInstance *JavaVmInstance::getInstance(JavaVM *vmPtr, int32_t jniVersion) {
    if (dxfeed::jni::internal::javaVM) {
        return dxfeed::jni::internal::javaVM;
    }
    return new JavaVmInstance(vmPtr, jniVersion);
}

JavaVmInstance::JavaVmInstance(JavaVM *vmPtr, const int jniVersion)
    : vm_(vmPtr), jniVersion_(jniVersion), criticalSection_() {
}

JavaVmInstance::~JavaVmInstance() {
    vm_->DestroyJavaVM();
    vm_ = nullptr;
}

JNIEnv *JavaVmInstance::getCurrenThread() {
    criticalSection_.enter();
    JNIEnv *env = nullptr;
    jint hr = vm_->GetEnv((void **)&env, jniVersion_);
    if (hr != JNI_OK) {
        if (hr == JNI_EDETACHED) {
            hr = attachCurrentThread(&env);
            if (hr != JNI_OK) {
                fprintf(stderr, "Can't attachCurrentThread. Exiting...\n");
            }
        } else {
            fprintf(stderr, "Can't getCurrenThread, error = %s\n", hrToMsg(hr));
        }
    }
    //    printf("getCurrenThread, env = %p\n", env);
    criticalSection_.leave();
    return env;
}

int32_t JavaVmInstance::attachCurrentThread(JNIEnv **env) {
    criticalSection_.enter();
    jint hr = vm_->AttachCurrentThread((void **)env, nullptr);
    if (hr == JNI_OK) {
        //      printf("New thread is attached. Env = %p, tid: \n", *env); // todo: getPid cross-platform
    } else {
        logHRESULT(*env, hr);
    }
    criticalSection_.leave();
    return hr;
}

void JavaVmInstance::detachCurrentThread() {
    criticalSection_.enter();
    static thread_local DetachJniThreadOnExit onExit{vm_};
    criticalSection_.leave();
}

void JavaVmInstance::logErrMsg(JNIEnv *env, jint hr, const char *errMsg) {
    fprintf(stderr, "ENV = %p, hr = %s, err = %s", env, hrToMsg(hr), errMsg);
}

void JavaVmInstance::logHRESULT(JNIEnv *env, jint hr) {
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

const char *JavaVmInstance::hrToMsg(int hr) {
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

JavaVmInstance::DetachJniThreadOnExit::DetachJniThreadOnExit(JavaVM *vmPtr) : vmCached_(vmPtr) {
}

JavaVmInstance::DetachJniThreadOnExit::~DetachJniThreadOnExit() {
    //    printf("Detaching thread tid: \n" /** PID */); // todo: getPid cross-platform
    jint hr = vmCached_->DetachCurrentThread();
    if (hr != JNI_OK) {
        fprintf(stderr, "hr = %s", hrToMsg(hr));
    }
}
} // namespace dxfeed::jni::internal::vm
