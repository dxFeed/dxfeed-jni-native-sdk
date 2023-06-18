// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_DXFEEDJNICLASS_H
#define NATIVE_JNI_SDK_DXFEEDJNICLASS_H

#include <jni.h>

namespace dxfeed::jni {
  struct DxFeedJniClass {
    explicit DxFeedJniClass(JNIEnv* env);
    _jclass* const clazz;
  };

}

#endif //NATIVE_JNI_SDK_DXFEEDJNICLASS_H
