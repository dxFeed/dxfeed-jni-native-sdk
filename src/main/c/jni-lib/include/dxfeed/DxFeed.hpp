// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_FEED_H_
#define DXFEED_JNI_NATIVE_SDK_FEED_H_

#include <string>
#include <jni.h>

namespace dxfeed {
  typedef void(*OnCloseHandler)(jobject clazz);
  void onClose(jobject);

  struct Connection; // forward declaration

  extern JNIEnv* jniEnv;
  extern JavaVM* javaVM;

  struct DxFeed final {
    DxFeed(const DxFeed& other) = delete;
    DxFeed(DxFeed&& other) = delete;
    DxFeed& operator=(const DxFeed& other) = delete;
    DxFeed& operator=(DxFeed&& other) = delete;

    static DxFeed& getInstance();
    jclass helperClass();
    jmethodID addEventListenerMethod();

    Connection* createConnection(const std::string &address);
  private:
    DxFeed();

    JNIEnv* env_;
    jclass javaHelperClass_;
    jmethodID addEventListenerHelperMethodId_;

  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_FEED_H_