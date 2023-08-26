// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_system.h"
#include "dxfeed/utils/JNICommon.hpp"

int32_t dxfg_system_set_property(graal_isolatethread_t* thread, const char* key, const char* value) {
  const auto* pSystem = dxfeed::jni::internal::javaLangSystem;
  pSystem->setProperty(thread, key, value);
  return 0;
}

const char* dxfg_system_get_property(graal_isolatethread_t* thread, const char* key) {
  const auto* pSystem = dxfeed::jni::internal::javaLangSystem;
  return pSystem->getProperty(thread, key);
}

int32_t dxfg_system_release_property(graal_isolatethread_t* thread, const char* value) {
  delete[] value;
  return 0;
}