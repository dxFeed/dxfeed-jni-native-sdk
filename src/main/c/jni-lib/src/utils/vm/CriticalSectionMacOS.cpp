// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/vm/CriticalSection.hpp"

namespace dxfeed::jni::internal {
  CriticalSection::CriticalSection() {
    pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE);
    int hr = pthread_mutex_init(&criticalSection, &mAttr);
  }

  CriticalSection::~CriticalSection() {
    pthread_mutexattr_destroy(&mAttr);
  }

  void CriticalSection::enter() {
    pthread_mutex_lock(&criticalSection);
  }

  void CriticalSection::leave() {
    pthread_mutex_unlock(&criticalSection);
  }
}