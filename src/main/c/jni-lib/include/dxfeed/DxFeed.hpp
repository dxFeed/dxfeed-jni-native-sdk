// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_FEED_H_
#define DXFEED_JNI_NATIVE_SDK_FEED_H_

#include <string>

#include <jni.h>

extern "C" JNIEXPORT void JNICALL Java_com_dxfeed_api_JniTest_nOnQuoteEventListener(JNIEnv* env, jclass,
            jint size, jbyteArray eventList, jdoubleArray jDoubleData, jlong userCallback);

namespace dxfeed {
  typedef void(*OnCloseHandler)(jobject clazz);

  struct Connection; // forward declaration

  extern JNIEnv* jniEnv;
  extern JavaVM* javaVM;

  static JNINativeMethod methods[] = {
    {"nOnQuoteEventListener", "(I[B[DJ)V", (void*) &Java_com_dxfeed_api_JniTest_nOnQuoteEventListener },
  };

  namespace jni {
    jclass safeFindClass(JNIEnv* env, const char* clazzName);

    typedef jmethodID (JNIEnv::*JMethodIdProvider)(jclass, const char*, const char*);
    jmethodID safeGetMethod(JNIEnv*, JMethodIdProvider, jclass, const char* methodName, const char* signature);
    jmethodID safeGetStaticMethodID(JNIEnv*, jclass, const char* methodName, const char* signature);
    jmethodID safeGetMethodID(JNIEnv* env, jclass, const char* methodName, const char* signature);
  }  // namespace dxfeed::jni

  struct DxFeed final {
    static void initJavaVM(const char* javaHome, const char** vmOptions = nullptr, int vmArgsCount = 0);
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

    static void onClose(jobject);
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_FEED_H_