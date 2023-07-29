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
                                                     dxfg_event_clazz_list_t* eventClazzes)
  {
    return new DxSubscription(env, connection, eventClazzes, false);
  }


  DxTimeSeriesSubscription* DxSubscription::createTimeSeriesSubscription(JNIEnv* env, jobject connection,
                                                                                         dxfg_event_clazz_t eventType)
  {
    return new DxSubscription(env, connection, eventType, true);
  }

  DxTimeSeriesSubscription* DxSubscription::createTimeSeriesSubscription(JNIEnv* env, jobject connection,
                                                                                         dxfg_event_clazz_list_t* eventClazzes)
  {
    return new DxSubscription(env, connection, eventClazzes, true);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_t eventType, bool isTimeSeries) {
    jclass dxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass eventTypeClass = safeFindClass(env, className);
    jmethodID createSubscriptionId = getMethodId(env, dxFeedClass, isTimeSeries, false);
    jobject pDxSubscription = env->CallObjectMethod(dxFeed, createSubscriptionId, eventTypeClass);
    subscription_ = env->NewGlobalRef(pDxSubscription);
    env->DeleteLocalRef(pDxSubscription);
    env->DeleteLocalRef(eventTypeClass);
    env->DeleteLocalRef(dxFeedClass);
  }

  jmethodID DxSubscription::getMethodId(JNIEnv* env, jclass clazz, bool isTimeSeries, bool argIsArray) {
    const char* subscriptionSignature = argIsArray
      ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;"
      : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;";
    const char* timeSeriesSubscriptionSignature = argIsArray
      ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;"
      : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;";
    return isTimeSeries
          ? safeGetMethodID(env, clazz, "createSubscription", subscriptionSignature)
          : safeGetMethodID(env, clazz, "createTimeSeriesSubscription", timeSeriesSubscriptionSignature);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_list_t* eventClazzes, bool isTimeSeries) {
    jclass dxFeedClass = env->GetObjectClass(dxFeed);
    int32_t size = eventClazzes->size;
    jclass clazzArrayClass = env->FindClass("Ljava/lang/Class;");
    javaLogger->info("clazzArrayClass: %", clazzArrayClass);
    jobjectArray array = env->NewObjectArray(size, clazzArrayClass, nullptr);
    javaLogger->info("array: %", array);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClazzes->elements[i];
      javaLogger->info("\t pClazz = %, *pClazz = ", pClazz, *pClazz);
      const char* className = getEventClassType(*pClazz);
      javaLogger->info("\t array[\"%\"] = %", i, className);
      jclass eventTypeClass = safeFindClass(env, className);
      javaLogger->info("\teventTypeClass = %", eventTypeClass);
      env->SetObjectArrayElement(array, i, eventTypeClass);
    }
    jmethodID createSubscriptionId2 = getMethodId(env, dxFeedClass, isTimeSeries, true);
    javaLogger->info("createSubscriptionId2: %", createSubscriptionId2);
    jobject pDxSubscription = env->CallObjectMethod(dxFeed, createSubscriptionId2, array);
    javaLogger->info("pDxSubscription: %", pDxSubscription);

    subscription_ = env->NewGlobalRef(pDxSubscription);
    env->DeleteLocalRef(array);
    env->DeleteLocalRef(array); //todo delete array elements?
    env->DeleteLocalRef(clazzArrayClass);
    env->DeleteLocalRef(dxFeedClass);
  }

  DxSubscription::~DxSubscription() {
    internal::jniEnv->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(JNIEnv* env, DxEventListener* listener) const {
    auto dxSymbolClass = internal::dxJni->dxSubscriptionJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, "addEventListener",
                                               "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  void DxSubscription::removeListener(JNIEnv* env, DxEventListener* listener) const {
    auto dxSymbolClass = internal::dxJni->dxSubscriptionJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, "removeEventListener",
                                               "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  int32_t DxSubscription::addSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) const {
    jclass dxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = safeGetMethodID(env, dxFeedSubscriptionClass, "addSymbols",
                                                         "(Ljava/lang/Object;)V");
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, addSymbolsMethodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(dxFeedSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) const {
    int32_t size = symbols->size;
    jclass objectArrayClass = env->FindClass("Ljava/lang/Object;");
    jobjectArray array = env->NewObjectArray(size, objectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(array, i, jSymbol);
    }

    jclass dxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = safeGetMethodID(env, dxFeedSubscriptionClass, "addSymbols",
                                                   "([Ljava/lang/Object;)V");
    
    env->CallVoidMethod(subscription_, addSymbolsMethodId, array);
    env->DeleteLocalRef(dxFeedSubscriptionClass);
    env->DeleteLocalRef(objectArrayClass);
    env->DeleteLocalRef(array);
    return JNI_OK;
  }

  int32_t DxSubscription::setSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) const {
    jclass dxFeedSubscriptionClass = env->GetObjectClass(subscription_);
    jmethodID setSymbolsMethodId = safeGetMethodID(env, dxFeedSubscriptionClass, "setSymbols","([Ljava/lang/Object;)V");
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, setSymbolsMethodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(dxFeedSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  void DxSubscription::close(JNIEnv* env) const {
    jclass dxFeedSubscription = env->GetObjectClass(subscription_);
    jmethodID closeMethodId = safeGetMethodID(env, dxFeedSubscription, "close", "()V");
    env->CallVoidMethod(subscription_, closeMethodId);
    env->DeleteLocalRef(dxFeedSubscription);
  }
}