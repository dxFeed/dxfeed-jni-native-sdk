// SPDX-License-Identifier: MPL-2.0

#include <jni.h>
#include <iostream>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  const char* getEventClassType(dxfg_event_clazz_t eventType) {
    switch (eventType) {
      case DXFG_EVENT_QUOTE:
        return "Lcom/dxfeed/event/market/Quote;";
      case DXFG_EVENT_PROFILE:
        return "Lcom/dxfeed/event/market/Profile;";
      case DXFG_EVENT_SUMMARY:
        return "Lcom/dxfeed/event/market/Summary;";
      case DXFG_EVENT_GREEKS:
        return "Lcom/dxfeed/event/option/Greeks;";
      case DXFG_EVENT_CANDLE:
        return "Lcom/dxfeed/event/candle/Candle;";
      case DXFG_EVENT_DAILY_CANDLE:
        return "Lcom/dxfeed/event/candle/DailyCandle;";
      case DXFG_EVENT_UNDERLYING:
        return "Lcom/dxfeed/event/option/Underlying;";
      case DXFG_EVENT_THEO_PRICE:
        return "Lcom/dxfeed/event/option/TheoPrice;";
      case DXFG_EVENT_TRADE:
        return "Lcom/dxfeed/event/market/Trade;";
      case DXFG_EVENT_TRADE_ETH:
        return "Lcom/dxfeed/event/market/TradeETH;";
      case DXFG_EVENT_CONFIGURATION:
        return "Lcom/dxfeed/event/misc/Configuration;";
      case DXFG_EVENT_MESSAGE:
        return "Lcom/dxfeed/event/misc/Message;";
      case DXFG_EVENT_TIME_AND_SALE:
        return "Lcom/dxfeed/event/market/TimeAndSale;";
      case DXFG_EVENT_ORDER_BASE:
        return "Lcom/dxfeed/event/market/OrderBase;";
      case DXFG_EVENT_ORDER:
        return "Lcom/dxfeed/event/market/Order;";
      case DXFG_EVENT_ANALYTIC_ORDER:
        return "Lcom/dxfeed/event/market/AnalyticOrder;";
      case DXFG_EVENT_SPREAD_ORDER:
        return "Lcom/dxfeed/event/market/SpreadOrder;";
      case DXFG_EVENT_SERIES:
        return "Lcom/dxfeed/event/option/Series;";
      case DXFG_EVENT_OPTION_SALE: // todo: there is no class in QD
      default:
        return nullptr;
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

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_list_t* eventClazzes) :
    env_{env}
  {
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
      jclass eventTypeClass = jni::safeFindClass(env, className);
      std::cout << "\teventTypeClass = " << eventTypeClass << "\n";
      env->SetObjectArrayElement(array, i, eventTypeClass);
    }
    jmethodID createSubscriptionId2 = jni::safeGetMethodID(env, dxFeedClass, "createSubscription",
                                                           "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;");
    std::cout << "createSubscriptionId2:" << createSubscriptionId2 << "\n";
    jobject sub = env->CallObjectMethod(dxFeed, createSubscriptionId2, array);
    std::cout << "sub:" << sub << "\n";

    subscription_ = env->NewGlobalRef(sub);
    env_->DeleteLocalRef(array); //todo delete array elements?
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