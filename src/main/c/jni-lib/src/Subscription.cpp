// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "api/Subscription.hpp"
#include "api/DxFeed.hpp"

namespace dxfeed {
  const char* getEventClassType(EventType eventType) {
    switch (eventType) {
      case EventType::TimeAndSale:
        return "Lcom/dxfeed/event/market/TimeAndSale;";
    }
  }

  jobject DxFeedCApi_createSubscription(JNIEnv* env, jobject dxFeed, EventType eventType) {
    jclass dxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass eventTypeClass = jni::safeFindClass(env, className);
    jmethodID connectMethodId = jni::safeGetMethodID(env, dxFeedClass, "createSubscription",
                                                 "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;");
    return env->CallObjectMethod(dxFeed, connectMethodId, eventTypeClass);
  }

  Subscription::Subscription(JNIEnv* env, jobject dxFeed, EventType eventType, dxfeed::OnCloseHandler onClose) :
    env_{env},
    onClose_(onClose)
  {
    subscription_ = env->NewGlobalRef(DxFeedCApi_createSubscription(env, dxFeed, eventType));
  }

  Subscription::~Subscription() {
    onClose_(subscription_);
  }

  void Subscription::addListener(Listener listener) const {
    auto& feed = dxfeed::DxFeed::getInstance();
    env_->CallStaticVoidMethod(feed.helperClass(), feed.addEventListenerMethod(),
                               subscription_, reinterpret_cast<jlong>(listener));
  }

  void Subscription::addSymbol(const std::string& symbol) const {
    jclass dxFeedSubscription = env_->GetObjectClass(subscription_);
    jmethodID addSymbolsMethodId = jni::safeGetMethodID(env_, dxFeedSubscription, "addSymbols", "(Ljava/lang/Object;)V");
    jstring pSymbol = env_->NewStringUTF(symbol.c_str());
    env_->CallVoidMethod(subscription_, addSymbolsMethodId, pSymbol);
  }

  void Subscription::addDiagnosticListener(int64_t listener) const {
    auto& feed = dxfeed::DxFeed::getInstance();
    env_->CallStaticVoidMethod(feed.helperClass(), feed.addEventListenerMethod(),
                               subscription_, static_cast<jlong>(listener));
  }
}