// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

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
} // namespace dxfeed::jni::internal