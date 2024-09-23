// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/DxThreadException.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
DxThreadException::DxThreadException(JNIEnv *env) {
    dxThreadExceptionClass_ = (jclass)env->NewGlobalRef(safeFindClass(env, "com/dxfeed/api/DxThreadException"));
    //    fprintf(stdout, "com.dxfeed.api.DxThreadException: 0x%p\n", dxThreadExceptionClass_);
    getExceptionInfo_ =
        safeGetStaticMethodID(env, dxThreadExceptionClass_, "getExceptionInfo", "(Ljava/lang/Throwable;)[B");
    //    fprintf(stdout, "byte[] DxThreadException::getExceptionInfo(Throwable t): 0x%p\n", getExceptionInfo_);
}

DxThreadException::~DxThreadException() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(dxThreadExceptionClass_);
    getExceptionInfo_ = nullptr;
}

// can be called from any Java thread, so dxThreadExceptionClass_ must be global reference
dxfg_exception_t *DxThreadException::getAndClearThreadException(JNIEnv *env) {
    auto wasException = env->ExceptionCheck();
    if (wasException) {
        jthrowable jThrowable = env->ExceptionOccurred();
        if (jThrowable) {
            auto jArray =
                r_cast<jarray>(env->CallStaticObjectMethod(dxThreadExceptionClass_, getExceptionInfo_, jThrowable));
            jthrowable pJthrowable = env->ExceptionOccurred();
            if (pJthrowable) {
                fprintf(stderr, "THROWABLE in getAndClearThreadException\n");
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            //        jsize arrayLength = env->GetArrayLength(jArray);
            auto pData = r_cast<char *>(env->GetPrimitiveArrayCritical(jArray, 0));

            int pos = 0;
            const char *className = copy(pData + pos);
            pos += strlen(className) + 1;
            const char *message = copy(pData + pos);
            pos += strlen(message) + 1;
            const char *stackTrace = copy(pData + pos);
            //        pos += strlen(stackTrace) + 1;
            //        dxfg_exception_t* cause = nullptr;
            //        if (pos < arrayLength) {
            //          cause = getAndClearThreadException(env);
            //        }

            env->ReleasePrimitiveArrayCritical(jArray, pData, 0);
            env->ExceptionClear();
            return new dxfg_exception_t{className, message, stackTrace, nullptr, nullptr};
        }
    }
    return nullptr;
}
} // namespace dxfeed::jni