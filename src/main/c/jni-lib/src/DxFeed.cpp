// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxEventT.hpp"
#include "dxfeed/utils/NativeEventReader.hpp"
#include "dxfeed/utils/NativeEventData.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed {
  using namespace jni;

  DxFeed::DxFeed(JNIEnv* env, jobject dxFeed) :
      dxFeed_(env->NewGlobalRef(dxFeed)),
      dxFeedClass_(env->GetObjectClass(dxFeed))
  {}

  DxFeed::~DxFeed() {
    internal::jniEnv->DeleteGlobalRef(dxFeed_);
  }

  // todo: make singleton
  dxfg_feed_t* DxFeed::getInstance(graal_isolatethread_t* env) {
    const auto dxFeedJniClazz = internal::dxJni->dxFeedJniClass_;
    jmethodID getInstanceId = safeGetStaticMethodID(env, dxFeedJniClazz, "getInstance", "()Lcom/dxfeed/api/DXFeed;");
    jobject dxFeedObject = env->CallStaticObjectMethod(dxFeedJniClazz, getInstanceId);
    return dxfeed::r_cast<dxfg_feed_t*>(new DxFeed(env, dxFeedObject));
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_t eventType) {
    return DxSubscription::createSubscription(env, dxFeed_, eventType);
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventClazzes) {
    return DxSubscription::createSubscription(env, dxFeed_, eventClazzes);
  }

  DxTimeSeriesSubscription* DxFeed::createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_t eventClazzes) {
    return DxSubscription::createTimeSeriesSubscription(env, dxFeed_, eventClazzes);
  }

  DxTimeSeriesSubscription* DxFeed::createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType) {
    return DxSubscription::createTimeSeriesSubscription(env, dxFeed_, eventType);
  }

  void DxFeed::attachSubscription(graal_isolatethread_t* env, dxfg_subscription_t* pSubscription) {
    jmethodID attachSubscriptionId = safeGetMethodID(env, dxFeedClass_, "attachSubscription",
                                                          "(Lcom/dxfeed/api/DXFeedSubscription;)V;");
    env->CallVoidMethod(dxFeed_, attachSubscriptionId, pSubscription);
  }

  void DxFeed::detachSubscription(graal_isolatethread_t* env, dxfg_subscription_t* pSubscription) {
    jmethodID detachSubscriptionId = safeGetMethodID(env, dxFeedClass_, "detachSubscription",
                                                          "(Lcom/dxfeed/api/DXFeedSubscription;)V;");
    env->CallVoidMethod(dxFeed_, detachSubscriptionId, pSubscription);
  }

  void DxFeed::detachSubscriptionAndClear(graal_isolatethread_t* env, dxfg_subscription_t* pSubscription) {
    jmethodID detachSubscriptionId = safeGetMethodID(env, dxFeedClass_, "detachSubscriptionAndClear",
                                                          "(Lcom/dxfeed/api/DXFeedSubscription;)V;");
    env->CallVoidMethod(dxFeed_, detachSubscriptionId, pSubscription);
  }

  dxfg_event_type_t* DxFeed::getLastEventIfSubscribed(graal_isolatethread_t* env, dxfg_event_clazz_t eventTypeClass,
                                                      dxfg_symbol_t* pSymbolType)
  {
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbolType);

    const char* className = getEventClassType(eventTypeClass);
    jclass eventTypeClazz = safeFindClass(env, className);

    const auto dxJniClazz = internal::dxJni->dxFeedJniClass_;
    jmethodID getLastEventIfSubscribedId =
      safeGetStaticMethodID(env, dxJniClazz, "getLastEventIfSubscribed",
                            "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/Object;)Lcom/dxfeed/api/DxFeedJni$NativeEventData;");

    jobject nativeEventData = env->CallStaticObjectMethod(dxJniClazz, getLastEventIfSubscribedId, dxFeed_,
                                                          eventTypeClazz, jSymbol);
    dxfg_event_type_t* pEventType = nullptr;
    if (nativeEventData) {
      NativeEventData data{env};
      data.toNativeEvent(nativeEventData, &pEventType);
    }

    env->DeleteLocalRef(nativeEventData);
    env->DeleteLocalRef(eventTypeClazz);
    env->DeleteLocalRef(jSymbol);

    return pEventType;
  }

  void DxFeed::getLastEvent(graal_isolatethread_t* env, dxfg_event_type_t* pEventType) {
    const auto dxFeedJniClazz = internal::dxJni->dxFeedJniClass_;
    const char* getLastEventSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/String;)Lcom/dxfeed/api/DxFeedJni$NativeEventData;";
    jmethodID getLastEventId = safeGetStaticMethodID(env, dxFeedJniClazz, "getLastEvent", getLastEventSignature);

    auto dxEventT = r_cast<DxEventT*>(pEventType);
    jstring jSymbolName = env->NewStringUTF(dxEventT->symbol_);
    const char* className = getEventClassType(dxEventT->eventType_.clazz);
    jclass eventTypeClass = safeFindClass(env, className);
    jobject nativeEventData = env->CallStaticObjectMethod(dxFeedJniClazz, getLastEventId, dxFeed_,
                                                          jSymbolName, eventTypeClass);
    if (nativeEventData) {
      NativeEventData data{env};
      data.toNativeEvent(nativeEventData, &pEventType);
    }

    env->DeleteLocalRef(nativeEventData);
    env->DeleteLocalRef(eventTypeClass);
    env->DeleteLocalRef(jSymbolName);
  }

  void DxFeed::getLastEvents(graal_isolatethread_t* env, dxfg_event_type_list* pList) {
    const auto dxFeedJniClazz = internal::dxJni->dxFeedJniClass_;
    const char* getLastEventSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/String;)Lcom/dxfeed/api/DxFeedJni$NativeEventData;";
    jmethodID getLastEventId = safeGetStaticMethodID(env, dxFeedJniClazz, "getLastEvent", getLastEventSignature);

    for (int i = 0; i < pList->size; ++i) {
      dxfg_event_type_t* pEventType = pList->elements[i];
      auto dxEventT = r_cast <DxEventT*>(pEventType);
      jstring jSymbolName = env->NewStringUTF(dxEventT->symbol_);
      const char* className = getEventClassType(dxEventT->eventType_.clazz);
      jclass eventTypeClass = safeFindClass(env, className);
      jobject nativeEventData = env->CallStaticObjectMethod(dxFeedJniClazz, getLastEventId, dxFeed_,
                                                            jSymbolName, eventTypeClass);
      if (nativeEventData) {
        NativeEventData data{env};
        data.toNativeEvent(nativeEventData, &pEventType);
      }
      env->DeleteLocalRef(nativeEventData);
      env->DeleteLocalRef(eventTypeClass);
      env->DeleteLocalRef(jSymbolName);
    }
  }

  const char* getEventClassType(dxfg_event_clazz_t eventTypeClazz) {
    switch (eventTypeClazz) {
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
}
