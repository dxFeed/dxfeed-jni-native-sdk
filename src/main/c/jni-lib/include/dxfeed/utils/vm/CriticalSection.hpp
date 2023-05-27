// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_CRITICALSECTION_H_
#define DXFEED_JNI_NATIVE_SDK_CRITICALSECTION_H_

#if _MSC_VER && !__INTEL_COMPILER
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace dxfeed::jni::internal {
  struct CriticalSection {
    explicit CriticalSection();
    void enter();
    void leave();
    ~CriticalSection();

#if _MSC_VER && !__INTEL_COMPILER
  CRITICAL_SECTION criticalSection = {};
#else
  pthread_mutex_t criticalSection = {}; // todo: init
  pthread_mutexattr_t mAttr;
#endif
};
}

#endif //DXFEED_JNI_NATIVE_SDK_CRITICALSECTION_H_