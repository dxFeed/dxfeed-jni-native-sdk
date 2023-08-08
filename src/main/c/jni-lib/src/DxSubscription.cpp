// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed {
  using namespace jni;

  DxSubscription::DxSubscription(JNIEnv* env, dxfg_event_clazz_t eventType) {
    dxSubscriptionClass_ = safeFindClass(env, "Lcom/dxfeed/api/DXFeedSubscription;");
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, "<init>", "(Ljava/lang/Class;)V");
    const char* className = getEventClassType(eventType);
    jclass jEventTypeClass = safeFindClass(env, className);
    jobject jDxSubscription = env->NewObject(dxSubscriptionClass_, methodId, jEventTypeClass);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, dxfg_event_clazz_t, bool), jDxSubscription: %", subscription_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventClasses) {
    dxSubscriptionClass_ = safeFindClass(env, "Lcom/dxfeed/api/DXFeedSubscription;");
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, "<init>", "([Ljava/lang/Class;)V");

    int32_t size = eventClasses->size;
    jclass jClazzArrayClass = env->FindClass("Ljava/lang/Class;");
    jobjectArray jArray = env->NewObjectArray(size, jClazzArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClasses->elements[i];
      const char* className = getEventClassType(*pClazz);
      jclass jEventTypeClass = safeFindClass(env, className);
      env->SetObjectArrayElement(jArray, i, jEventTypeClass);
      env->DeleteLocalRef(jEventTypeClass);
    }
    jobject jDxSubscription = env->NewObject(dxSubscriptionClass_, methodId, jArray);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, dxfg_event_clazz_list_t*, bool), jDxSubscription: %", dxSubscriptionClass_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jClazzArrayClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_t eventType, bool isTimeSeries) {
    dxSubscriptionClass_ = safeFindClass(env, "Lcom/dxfeed/api/DXFeedSubscription;");
    jclass jDxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass jEventTypeClass = safeFindClass(env, className);
    jmethodID methodId = getMethodId(env, jDxFeedClass, isTimeSeries, false);
    jobject jDxSubscription = env->CallObjectMethod(dxFeed, methodId, jEventTypeClass);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, jobject, dxfg_event_clazz_t, bool), jDxSubscription: %",
                     dxSubscriptionClass_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_list_t* eventClasses, bool isTimeSeries) {
    dxSubscriptionClass_ = safeFindClass(env, "Lcom/dxfeed/api/DXFeedSubscription;");
    jclass jDxFeedClass = env->GetObjectClass(dxFeed);
    int32_t size = eventClasses->size;
    jclass jClazzArrayClass = env->FindClass("Ljava/lang/Class;");
    jobjectArray jArray = env->NewObjectArray(size, jClazzArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClasses->elements[i];
      const char* className = getEventClassType(*pClazz);
      jclass jEventTypeClass = safeFindClass(env, className);
      env->SetObjectArrayElement(jArray, i, jEventTypeClass);
      env->DeleteLocalRef(jEventTypeClass);
    }
    jmethodID methodId = getMethodId(env, jDxFeedClass, isTimeSeries, true);
    jobject jDxSubscription = env->CallObjectMethod(dxFeed, methodId, jArray);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, jobject, dxfg_event_clazz_list_t*, bool), jDxSubscription: %",
                     subscription_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jClazzArrayClass);
    env->DeleteLocalRef(jDxFeedClass);
  }

  jmethodID DxSubscription::getMethodId(JNIEnv* env, jclass clazz, bool isTimeSeries, bool argIsArray) {
    const char* methodName;
    const char* signature;
    if (isTimeSeries) {
      methodName = "createTimeSeriesSubscription";
      signature = argIsArray  ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;"
                              : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;";
    } else {
      methodName = "createSubscription";
      signature = argIsArray  ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;"
                              : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;";
    }
    return safeGetMethodID(env, clazz, methodName, signature);
  }

  DxSubscription::~DxSubscription() {
    internal::jniEnv->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(JNIEnv* env, DxEventListener* listener) {
    auto dxSymbolClass = internal::dxJni->dxSubscriptionJniClass_;
    const char* methodName = "addEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, methodName, methodSignature);
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  void DxSubscription::removeListener(JNIEnv* env, DxEventListener* listener) {
    auto dxSymbolClass = internal::dxJni->dxSubscriptionJniClass_;
    const char* methodName = "removeEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, methodName, methodSignature);
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  int32_t DxSubscription::addSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    const char* methodName = "addSymbols";
    const char* methodSignature = "(Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = env->FindClass("Ljava/lang/Object;");
    jobjectArray jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    const char* methodName = "addSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName,methodSignature);
    env->CallVoidMethod(subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    return JNI_OK;
  }

  int32_t DxSubscription::setSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    const char* methodName = "setSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::setSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = env->FindClass("Ljava/lang/Object;");
    jobjectArray jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    const char* methodName = "setSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName,methodSignature);
    env->CallVoidMethod(subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
  }

  void DxSubscription::close(JNIEnv* env) {
    const char* methodName = "close";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId);
  }

  int32_t DxSubscription::setTime(graal_isolatethread_t* env, int64_t time) {
    const char* methodName = "setTime";
    const char* methodSignature = "(J)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, time);
    return JNI_OK;
  }

  int32_t DxSubscription::removeSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    const char* methodName = "removeSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::removeSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = env->FindClass("Ljava/lang/Object;");
    jobjectArray jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    const char* methodName = "removeSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    return JNI_OK;
  }

  int32_t DxSubscription::attach(JNIEnv* env, DxFeed* pFeed) {
    const char* methodName = "attach";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeed;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, pFeed->getJavaObject());
    return JNI_OK;
  }

  int32_t DxSubscription::detach(JNIEnv* env, DxFeed* pFeed) {
    const char* methodName = "detach";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeed;)V";
    jmethodID methodId = safeGetMethodID(env, dxSubscriptionClass_, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, pFeed->getJavaObject());
    return JNI_OK;
  }
}