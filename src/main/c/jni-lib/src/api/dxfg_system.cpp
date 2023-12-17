// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/dxfg_system.h"
#include "dxfeed/utils/JNICommon.hpp"

int32_t dxfg_system_set_property(graal_isolatethread_t* thread, const char* key, const char* value) {
  auto* pSystem = dxfeed::jni::internal::javaLangSystem;
  pSystem->setProperty(thread, key, value);
  return JNI_OK;
}

const char* dxfg_system_get_property(graal_isolatethread_t* thread, const char* key) {
  const auto* pSystem = dxfeed::jni::internal::javaLangSystem;
  return pSystem->getProperty(thread, key);
}

int32_t dxfg_system_release_property(graal_isolatethread_t* thread, const char* value) {
  delete[] value;
  return JNI_OK;
}