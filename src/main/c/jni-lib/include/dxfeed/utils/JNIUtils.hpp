#ifndef DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
#define DXFEED_JNI_NATIVE_SDK_JNIUTILS_H

#include <jni.h>

extern "C" JNIEXPORT void JNICALL Java_com_dxfeed_api_JniTest_nOnQuoteEventListener(JNIEnv* env, jclass,
                                                                                    jint size, jbyteArray eventList,
                                                                                    jdoubleArray jDoubleData,
                                                                                    jlong dxSubscription);
namespace dxfeed::jni {
  namespace nativeMethods {
    static JNINativeMethod methods[] = {
        {"nOnQuoteEventListener", "(I[B[DJ)V", (void*) &Java_com_dxfeed_api_JniTest_nOnQuoteEventListener},
    };
  }

    jclass safeFindClass(JNIEnv* env, const char* clazzName);

    void registerNativeMethods(JNIEnv* env, jclass clazz);

    typedef jmethodID (JNIEnv::*JMethodIdProvider)(jclass, const char*, const char*);
    jmethodID safeGetMethod(JNIEnv*, JMethodIdProvider, jclass, const char* methodName, const char* signature);
    jmethodID safeGetStaticMethodID(JNIEnv*, jclass, const char* methodName, const char* signature);
    jmethodID safeGetMethodID(JNIEnv* env, jclass, const char* methodName, const char* signature);
  }

#endif //DXFEED_JNI_NATIVE_SDK_JNIUTILS_H
