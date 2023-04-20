// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_FEED_H_
#define DXFEED_JNI_NATIVE_SDK_FEED_H_

#include <string>
#include <jni.h>

namespace dxfeed {
  typedef void(*OnCloseHandler)(jobject clazz);
  void onClose(jobject);

  extern JNIEnv* jniEnv;
  extern JavaVM* javaVM;

  struct DxfgContext final {
    DxfgContext(const DxfgContext& other) = delete;
    DxfgContext(DxfgContext&& other) = delete;
    DxfgContext& operator=(const DxfgContext& other) = delete;
    DxfgContext& operator=(DxfgContext&& other) = delete;

    static DxfgContext& getInstance();
    jclass helperClass();
    jmethodID addEventListenerMethod();

  private:
    DxfgContext();

    JNIEnv* env_;
    jclass javaHelperClass_;
    jmethodID addEventListenerHelperMethodId_;

  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_FEED_H_