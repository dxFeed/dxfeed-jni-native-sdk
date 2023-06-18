// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include "dxfeed/utils/java/DxFeedJniClass.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  DxFeedJniClass::DxFeedJniClass(JNIEnv* env):
    clazz(reinterpret_cast<_jclass* const>(env->NewGlobalRef(jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;"))))
  {
    std::cout << "com.dxfeed.api.DxFeedJniClass: " << clazz << "\n";
  }
}
