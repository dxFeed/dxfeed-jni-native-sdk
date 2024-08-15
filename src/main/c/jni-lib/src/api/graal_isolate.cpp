// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/graal_isolate.h"
#include "dxfeed/utils/JNIUtils.hpp"

int graal_create_isolate(graal_create_isolate_params_t *params, graal_isolate_t **isolate,
                         graal_isolatethread_t **thread) {
    if (dxfeed::jni::internal::javaVM == nullptr) {
        if (params) {
            auto vmOptions = dxfeed::r_cast<dxfeed::jni::VMOptions *>(params);
            dxfeed::jni::internal::initJavaVM(vmOptions);
        } else {
            dxfeed::jni::VMOptions vmOptions{dxfeed::jni::getJavaHomeFromEnv(), nullptr, 0};
            dxfeed::jni::internal::initJavaVM(&vmOptions);
        }
        *isolate = (void *)dxfeed::jni::internal::javaVM;
        *thread = dxfeed::jni::internal::jniEnv;
    }
    return (*isolate != nullptr && *thread != nullptr) ? JNI_OK : -1;
}

graal_isolatethread_t *graal_get_current_thread(graal_isolate_t *isolate) {
    auto vmInstance = dxfeed::r_cast<dxfeed::jni::internal::vm::JavaVmInstance *>(isolate);
    return vmInstance->getCurrenThread();
}

// https://github.com/oracle/graal/blob/8be56121aa31e7448b4adb0224ab2ac44095ed9b/compiler/src/jdk.internal.vm.compiler/src/org/graalvm/libgraal/LibGraal.java#L253
int graal_attach_thread(graal_isolate_t *isolate, graal_isolatethread_t **env) {
    auto vmInstance = dxfeed::r_cast<dxfeed::jni::internal::vm::JavaVmInstance *>(isolate);
    vmInstance->attachCurrentThread(env);
    return JNI_OK;
}

graal_isolate_t *graal_get_isolate(graal_isolatethread_t *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

// https://github.com/oracle/graal/blob/8be56121aa31e7448b4adb0224ab2ac44095ed9b/compiler/src/jdk.internal.vm.compiler/src/org/graalvm/libgraal/LibGraal.java#L283
// jint DetachCurrentThread()
int graal_detach_thread(graal_isolatethread_t *) {
    dxfeed::jni::internal::javaVM->detachCurrentThread();
    return JNI_OK;
}

int graal_tear_down_isolate(graal_isolatethread_t *) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int graal_detach_all_threads_and_tear_down_isolate(graal_isolatethread_t *) {
    return INVALID_INT_RESULT_MINUS_ONE;
}