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
  }

  DxSubscription::~DxSubscription() {
    env_->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(dxfg_feed_event_listener_t* listener) const {
    jclass javaHelperClass_ = jni::safeFindClass(env_, "Lcom/dxfeed/api/JniTest;");
    jmethodID addEventListenerHelperMethodId_ = jni::safeGetStaticMethodID(env_, javaHelperClass_, "addEventListener",
                                                                           "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
    env_->CallStaticVoidMethod(javaHelperClass_, addEventListenerHelperMethodId_,
                               subscription_, reinterpret_cast<jlong>(listener));
  }

  void DxSubscription::addSymbol(const std::string& symbol) const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "addSymbols", "(Ljava/lang/Object;)V");
    jstring pSymbol = env_->NewStringUTF(symbol.c_str());
    env_->CallVoidMethod(subscription_, addSymbolsMethodId, pSymbol);
  }

  void DxSubscription::setSymbol(const std::string& symbol) const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "setSymbols", "(Ljava/lang/Object;)V");
    jstring pSymbol = env_->NewStringUTF(symbol.c_str());
    env_->CallVoidMethod(subscription_, addSymbolsMethodId, pSymbol);
  }

  void DxSubscription::close() const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID closeMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "close", "()V");
    env_->CallVoidMethod(subscription_, closeMethodId);
  }
}