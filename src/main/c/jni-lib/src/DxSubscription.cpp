// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed {
  using namespace jni;

  DxSubscription* DxSubscription::createSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType) {
    return new DxSubscription(env, connection, eventType, false);
  }

  DxSubscription* DxSubscription::createSubscription(JNIEnv* env, jobject connection,
                                                     dxfg_event_clazz_list_t* eventClasses)
  {
    return new DxSubscription(env, connection, eventClasses, false);
  }


  DxTimeSeriesSubscription* DxSubscription::createTimeSeriesSubscription(JNIEnv* env, jobject connection,
                                                                         dxfg_event_clazz_t eventType)
  {
    return new DxSubscription(env, connection, eventType, true);
  }

  DxTimeSeriesSubscription* DxSubscription::createTimeSeriesSubscription(JNIEnv* env, jobject connection,
                                                                         dxfg_event_clazz_list_t* eventClasses)
  {
    return new DxSubscription(env, connection, eventClasses, true);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_t eventType, bool isTimeSeries) {
    jclass jDxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass jEventTypeClass = safeFindClass(env, className);
    jmethodID methodId = getMethodId(env, jDxFeedClass, isTimeSeries, false);
    jobject jDxSubscription = env->CallObjectMethod(dxFeed, methodId, jEventTypeClass);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedClass);
  }

  jmethodID DxSubscription::getMethodId(JNIEnv* env, jclass clazz, bool isTimeSeries, bool argIsArray) {
    const char* methodSignature;
    const char* methodName;
    if (isTimeSeries) {
      methodSignature = argIsArray  ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;"
                                    : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;";
      methodName = "createSubscription";
    } else {
      methodSignature = argIsArray  ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;"
                                    : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;";
      methodName = "createTimeSeriesSubscription";
    }

    return safeGetMethodID(env, clazz, methodName, methodSignature);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_list_t* eventClasses, bool isTimeSeries) {
    jclass jDxFeedClass = env->GetObjectClass(dxFeed);
    int32_t size = eventClasses->size;
    jclass jClazzArrayClass = env->FindClass("Ljava/lang/Class;");
//    javaLogger->info("jClazzArrayClass: %", jClazzArrayClass);
    jobjectArray jArray = env->NewObjectArray(size, jClazzArrayClass, nullptr);
//    javaLogger->info("jArray: %", jArray);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClasses->elements[i];
//      javaLogger->info("\t pClazz = %, *pClazz = ", pClazz, *pClazz);
      const char* className = getEventClassType(*pClazz);
//      javaLogger->info("\t jArray[\"%\"] = %", i, className);
      jclass jEventTypeClass = safeFindClass(env, className);
//      javaLogger->info("\tjEventTypeClass = %", jEventTypeClass);
      env->SetObjectArrayElement(jArray, i, jEventTypeClass);
      env->DeleteLocalRef(jEventTypeClass);
    }
    jmethodID methodId = getMethodId(env, jDxFeedClass, isTimeSeries, true);
    jobject jDxSubscription = env->CallObjectMethod(dxFeed, methodId, jArray);
    javaLogger->info("jDxSubscription: %", jDxSubscription);

    subscription_ = env->NewGlobalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jArray); //todo delete jArray elements?
    env->DeleteLocalRef(jClazzArrayClass);
    env->DeleteLocalRef(jDxFeedClass);
  }

  DxSubscription::~DxSubscription() {
    internal::jniEnv->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(JNIEnv* env, DxEventListener* listener) const {
    auto dxSymbolClass = internal::dxJni->dxSubscriptionJniClass_;
    const char* methodName = "addEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, methodName, methodSignature);
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  void DxSubscription::removeListener(JNIEnv* env, DxEventListener* listener) const {
    auto dxSymbolClass = internal::dxJni->dxSubscriptionJniClass_;
    const char* methodName = "removeEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, methodName, methodSignature);
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  int32_t DxSubscription::addSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) const {
    jclass jDxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "addSymbols";
    const char* methodSignature = "(Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxFeedSubscriptionClass, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jDxFeedSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) const {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = env->FindClass("Ljava/lang/Object;");
    jobjectArray jArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jArray, i, jSymbol);
    }

    jclass jDxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "addSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxFeedSubscriptionClass, methodName,methodSignature);
    
    env->CallVoidMethod(subscription_, methodId, jArray);
    env->DeleteLocalRef(jDxFeedSubscriptionClass);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jArray);
    return JNI_OK;
  }

  int32_t DxSubscription::setSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) const {
    jclass jDxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxFeedSubscriptionClass, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jDxFeedSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  void DxSubscription::close(JNIEnv* env) const {
    jclass jDxFeedSubscription = env->GetObjectClass(subscription_);
    const char* methodName = "close";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxFeedSubscription, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId);
    env->DeleteLocalRef(jDxFeedSubscription);
  }

  int32_t DxSubscription::setTime(graal_isolatethread_t* env, int64_t time) const {
    jclass jDxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setTime";
    const char* methodSignature = "(J)V";
    jmethodID methodId = safeGetMethodID(env, jDxFeedSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, time);    
    env->DeleteLocalRef(jDxFeedSubscriptionClass);
    return JNI_ERR;
  }
}