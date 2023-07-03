// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed {
  using namespace jni;

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_t eventType) {
    jclass dxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass eventTypeClass = safeFindClass(env, className);
    jmethodID createSubscriptionId = safeGetMethodID(env, dxFeedClass, "createSubscription",
                                                     "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;");
    jobject pDxSubscription = env->CallObjectMethod(dxFeed, createSubscriptionId, eventTypeClass);
    subscription_ = env->NewGlobalRef(pDxSubscription);
    env->DeleteLocalRef(pDxSubscription);
    env->DeleteLocalRef(eventTypeClass);
    env->DeleteLocalRef(dxFeedClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_list_t* eventClazzes) {
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
    jmethodID createSubscriptionId2 = safeGetMethodID(env, dxFeedClass, "createSubscription",
                                                           "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;");
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