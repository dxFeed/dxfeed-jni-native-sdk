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

  struct DxContext final {
    DxContext(const DxContext& other) = delete;
    DxContext(DxContext&& other) = delete;
    DxContext& operator=(const DxContext& other) = delete;
    DxContext& operator=(DxContext&& other) = delete;

    static DxContext& getInstance();
    jclass helperClass();
    jmethodID addEventListenerMethod();

  private:
    DxContext();

    JNIEnv* env_;
    jclass javaHelperClass_;
    jmethodID addEventListenerHelperMethodId_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_FEED_H_