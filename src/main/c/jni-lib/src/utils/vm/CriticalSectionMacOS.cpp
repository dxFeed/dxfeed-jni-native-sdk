// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/utils/vm/CriticalSection.hpp"

namespace dxfeed::jni::internal {
  CriticalSection::CriticalSection() {
    pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&criticalSection, &mAttr);
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