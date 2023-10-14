// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/DxThreadException.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  DxThreadException::DxThreadException(JNIEnv* env) {
    dxThreadExceptionClass_ = safeFindClass(env, "com/dxfeed/api/DxThreadException");
    javaLogger->info("com.dxfeed.api.DxThreadException: %", dxThreadExceptionClass_);
    getExceptionInfo_ = safeGetStaticMethodID(env, dxThreadExceptionClass_, "getExceptionInfo", "(Ljava/lang/Throwable;)[B");
    javaLogger->info("byte[] DxThreadException::getExceptionInfo(Throwable t): %", getExceptionInfo_);
  }

  DxThreadException::~DxThreadException() {
    dxThreadExceptionClass_ = nullptr;
    getExceptionInfo_ = nullptr;
  }

  const char* copy(const char* str) {
    auto len = static_cast<int32_t>(strlen(str));
    char* copiedData = new char[len + 1];
    copiedData[len] = 0;
    memcpy(copiedData, str, len);
    return copiedData;
  }

  dxfg_exception_t* DxThreadException::getAndClearThreadException(JNIEnv* env) {
    auto wasException = env->ExceptionCheck();
    if (wasException) {
      jthrowable jThrowable = env->ExceptionOccurred();
      if (jThrowable) {
        auto jArray = r_cast<jarray>(
          env->CallStaticObjectMethod(dxThreadExceptionClass_, getExceptionInfo_, jThrowable));
//        jsize arrayLength = env->GetArrayLength(jArray);
        auto pData = r_cast<char*>(env->GetPrimitiveArrayCritical(jArray, 0));

        int pos = 0;
        const char* className = copy(pData + pos);
        pos += strlen(className) + 1;
        const char* message = copy(pData + pos);
        pos += strlen(message) + 1;
        const char* stackTrace = copy(pData + pos);
//        pos += strlen(stackTrace) + 1;
//        dxfg_exception_t* cause = nullptr;
//        if (pos < arrayLength) {
//          cause = getAndClearThreadException(env);
//        }

        env->ReleasePrimitiveArrayCritical(jArray, pData, 0);
        env->ExceptionClear();
        return new dxfg_exception_t {className, message, stackTrace};
      }
    }
    return nullptr;
  }

  jclass DxThreadException::getJniClass() {
    return dxThreadExceptionClass_;
  }

}