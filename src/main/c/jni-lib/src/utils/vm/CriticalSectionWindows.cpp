// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/utils/vm/CriticalSection.hpp"

namespace dxfeed::jni::internal {
  CriticalSection::CriticalSection() {
    InitializeCriticalSection(&criticalSection);
  }

  CriticalSection::~CriticalSection() {
    DeleteCriticalSection(&criticalSection);
  }

  void CriticalSection::enter() {
    EnterCriticalSection(&criticalSection);
  }

  void CriticalSection::leave() {
    LeaveCriticalSection(&criticalSection);
  }
}