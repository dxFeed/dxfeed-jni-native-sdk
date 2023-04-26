// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  const char* getEventClassType(dxfg_event_clazz_t eventType) {
    switch (eventType) {
      case dxfg_event_clazz_t::DXFG_EVENT_TIME_AND_SALE:
        return "Lcom/dxfeed/event/market/TimeAndSale;";
      case dxfg_event_clazz_t::DXFG_EVENT_QUOTE:
        return "Lcom/dxfeed/event/market/Quote;";
    }
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_t eventType) :
    env_{env}
  {
    jclass dxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass eventTypeClass = jni::safeFindClass(env, className);
    jmethodID createSubscriptionId = jni::safeGetMethodID(env, dxFeedClass, "createSubscription",
                                                          "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;");
    subscription_ = env->NewGlobalRef(env->CallObjectMethod(dxFeed, createSubscriptionId, eventTypeClass));
    env_->DeleteLocalRef(eventTypeClass);
    env_->DeleteLocalRef(dxFeedClass);
  }

  DxSubscription::~DxSubscription() {
    env_->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(dxfg_feed_event_listener_t* listener) const {
    jclass pJclass = jni::safeFindClass(env_, "Lcom/dxfeed/api/JniTest;");
    jmethodID addEventListenerId = jni::safeGetStaticMethodID(env_, pJclass, "addEventListener",
                                                              "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
    env_->CallStaticVoidMethod(pJclass, addEventListenerId, subscription_, reinterpret_cast<jlong>(listener));
    env_->DeleteLocalRef(pJclass);
  }

  void DxSubscription::addSymbol(const std::string& symbol) const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "addSymbols", "(Ljava/lang/Object;)V");
    jstring pSymbol = env_->NewStringUTF(symbol.c_str());
    env_->CallVoidMethod(subscription_, addSymbolsMethodId, pSymbol);
    env_->DeleteLocalRef(pSymbol);
    env_->DeleteLocalRef(dxFeedSubscription);
  }

  void DxSubscription::setSymbol(const std::string& symbol) const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "setSymbols", "(Ljava/lang/Object;)V");
    jstring pSymbol = env_->NewStringUTF(symbol.c_str());
    env_->CallVoidMethod(subscription_, addSymbolsMethodId, pSymbol);
    env_->DeleteLocalRef(pSymbol);
    env_->DeleteLocalRef(dxFeedSubscription);
  }

  void DxSubscription::close() const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID closeMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "close", "()V");
    env_->CallVoidMethod(subscription_, closeMethodId);
    env_->DeleteLocalRef(dxFeedSubscription);
  }
}