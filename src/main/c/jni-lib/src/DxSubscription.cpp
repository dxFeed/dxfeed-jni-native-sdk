// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <iostream>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"

namespace dxfeed {
  using namespace jni;
  using namespace jni::internal;

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
    std::cout << "clazzArrayClass:" << clazzArrayClass << "\n";
    jobjectArray array = env->NewObjectArray(size, clazzArrayClass, nullptr);
    std::cout << "array:" << array << "\n";
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClazzes->elements[i];
      std::cout << "\tpClazz << = " << pClazz << ", *pClazz: " << *pClazz << "\n";
      const char* className = getEventClassType(*pClazz);
      std::cout << "\tarray[" << i << "] = " << className << "\n";
      jclass eventTypeClass = safeFindClass(env, className);
      std::cout << "\teventTypeClass = " << eventTypeClass << "\n";
      env->SetObjectArrayElement(array, i, eventTypeClass);
    }
    jmethodID createSubscriptionId2 = safeGetMethodID(env, dxFeedClass, "createSubscription",
                                                           "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;");
    std::cout << "createSubscriptionId2:" << createSubscriptionId2 << "\n";
    jobject pDxSubscription = env->CallObjectMethod(dxFeed, createSubscriptionId2, array);
    std::cout << "pDxSubscription:" << pDxSubscription << "\n";

    subscription_ = env->NewGlobalRef(pDxSubscription);
    env->DeleteLocalRef(array);
    env->DeleteLocalRef(array); //todo delete array elements?
    env->DeleteLocalRef(clazzArrayClass);
    env->DeleteLocalRef(dxFeedClass);
  }

  DxSubscription::~DxSubscription() {
    jniEnv->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(JNIEnv* env, DxEventListener* listener) const {
    auto dxSymbolClass = dxJni->dxSubscriptionJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, "addEventListener",
                                               "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  void DxSubscription::removeListener(JNIEnv* env, DxEventListener* listener) const {
    auto dxSymbolClass = dxJni->dxSubscriptionJniClass_;
    jmethodID methodId = safeGetStaticMethodID(env, dxSymbolClass, "removeEventListener",
                                               "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
    env->CallStaticVoidMethod(dxSymbolClass, methodId, subscription_, listener->javaListenerId_);
  }

  void DxSubscription::addSymbol(JNIEnv* env, dxfg_symbol_type_t symbolType, const std::string& symbol) const {
    auto dxSymbolClass = dxJni->dxSubscriptionJniClass_;
    jmethodID addSymbolsMethodId = safeGetStaticMethodID(env, dxSymbolClass, "addSymbol",
                                                         "(Lcom/dxfeed/api/DXFeedSubscription;ILjava/lang/String;)V");
    jstring pSymbol = env->NewStringUTF(symbol.c_str());
    env->CallStaticVoidMethod(dxSymbolClass, addSymbolsMethodId, subscription_, symbolType, pSymbol);
    env->DeleteLocalRef(pSymbol);
  }

  void DxSubscription::setSymbol(JNIEnv* env, dxfg_symbol_type_t symbolType, const std::string& symbol) const {
    auto dxSymbolClass = dxJni->dxSubscriptionJniClass_;
    jmethodID setSymbolsMethodId = safeGetStaticMethodID(env, dxSymbolClass, "setSymbol",
                                                         "(Lcom/dxfeed/api/DXFeedSubscription;ILjava/lang/String;)V");
    jstring pSymbol = env->NewStringUTF(symbol.c_str());
    env->CallStaticVoidMethod(dxSymbolClass, setSymbolsMethodId, subscription_, symbolType, pSymbol);
    env->DeleteLocalRef(pSymbol);
  }

  void DxSubscription::close(JNIEnv* env) const {
    jclass dxFeedSubscription = env->GetObjectClass(subscription_);
    jmethodID closeMethodId = safeGetMethodID(env, dxFeedSubscription, "close", "()V");
    env->CallVoidMethod(subscription_, closeMethodId);
    env->DeleteLocalRef(dxFeedSubscription);
  }
}