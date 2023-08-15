// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxEventT.hpp"
#include "dxfeed/utils/NativeEventReader.hpp"
#include "dxfeed/utils/NativeEventsList.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed {
  using namespace jni;

  DxFeed::DxFeed(JNIEnv* env, jobject dxFeed) :
      dxFeed_(env->NewGlobalRef(dxFeed))
  {}

  DxFeed::~DxFeed() {
    internal::jniEnv->DeleteGlobalRef(dxFeed_);
  }

  // todo: make singleton
  dxfg_feed_t* DxFeed::getInstance(JNIEnv* env) {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_CLASS_NAME);
    const char* methodName = "getInstance";
    const char* methodSignature = "()Lcom/dxfeed/api/DXFeed;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);
    jobject jDxFeedObject = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId);
    env->DeleteLocalRef(jDxFeedJniClazz);
    return dxfeed::r_cast<dxfg_feed_t*>(new DxFeed(env, jDxFeedObject));
  }

  jobject DxFeed::getJavaObject() const {
    return dxFeed_;
  }

  dxfg_event_type_t* DxFeed::getLastEventIfSubscribed(JNIEnv* env, dxfg_event_clazz_t eventTypeClass,
                                                      dxfg_symbol_t* pSymbolType)
  {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getLastEventIfSubscribed";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/Object;)Lcom/dxfeed/api/NativeEventsList;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    const char* className = getEventClassType(eventTypeClass);
    jclass jEventTypeClazz = safeFindClass(env, className);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbolType);
    jobject jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClazz, jSymbol);
    dxfg_event_type_t* pEventType = nullptr;
    if (jNativeEventsList) {
      NativeEventsList list {env};
      list.toNativeEvent(jNativeEventsList, &pEventType);
    }

    env->DeleteLocalRef(jNativeEventsList);
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jEventTypeClazz);
    env->DeleteLocalRef(jDxFeedJniClazz);

    return pEventType;
  }

  void DxFeed::getLastEvent(JNIEnv* env, dxfg_event_type_t* pEventType) {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getLastEvent";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/String;)Lcom/dxfeed/api/NativeEventList;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    auto dxEventT = r_cast<DxEventT*>(pEventType);
    const char* className = getEventClassType(dxEventT->eventType_.clazz);
    jclass jEventTypeClass = safeFindClass(env, className);
    jstring jSymbolName = env->NewStringUTF(dxEventT->symbol_);
    jobject jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jSymbolName,
                                                            jEventTypeClass);
    if (jNativeEventsList) {
      NativeEventsList list {env};
      list.toNativeEvent(jNativeEventsList, &pEventType);
    }

    env->DeleteLocalRef(jNativeEventsList);
    env->DeleteLocalRef(jSymbolName);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedJniClazz);
  }

  void DxFeed::getLastEvents(JNIEnv* env, dxfg_event_type_list* pList) {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getLastEvent";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/String;)Lcom/dxfeed/api/NativeEventsList;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    for (int i = 0; i < pList->size; ++i) {
      dxfg_event_type_t* pEventType = pList->elements[i];
      auto dxEventT = r_cast<DxEventT*>(pEventType);
      const char* className = getEventClassType(dxEventT->eventType_.clazz);
      jclass jEventTypeClass = safeFindClass(env, className);
      jstring jSymbolName = env->NewStringUTF(dxEventT->symbol_);
      jobject jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jSymbolName,
                                                              jEventTypeClass);
      if (jNativeEventsList) {
        NativeEventsList data{env};
        data.toNativeEvent(jNativeEventsList, &pEventType);
      }
      env->DeleteLocalRef(jNativeEventsList);
      env->DeleteLocalRef(jSymbolName);
      env->DeleteLocalRef(jEventTypeClass);
      env->DeleteLocalRef(jDxFeedJniClazz);
    }
  }

  dxfg_event_type_list* DxFeed::getIndexedEventsIfSubscribed(JNIEnv* env, dxfg_event_clazz_t clazz,
                                                             dxfg_symbol_t* pSymbolType, const char* source)
  {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getIndexedEventsIfSubscribed";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/Object;Ljava/lang/String;)Lcom/dxfeed/api/NativeEventsList;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    const char* className = getEventClassType(clazz);
    jclass jEventTypeClass = safeFindClass(env, className);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbolType);
    jstring jSource = env->NewStringUTF(source);
    jobject jNativesEventList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClass,
                                                            jSymbol, jSource);

    auto* resultEventList = new dxfg_event_type_list();
    if (jNativesEventList) {
      NativeEventsList list {env};
      list.toNativeEventsList(jNativesEventList, &resultEventList);
    }

    env->DeleteLocalRef(jNativesEventList);
    env->DeleteLocalRef(jSource);
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedJniClazz);
    return resultEventList;
  }

  dxfg_event_type_list* DxFeed::getTimeSeriesIfSubscribed(JNIEnv* env, dxfg_event_clazz_t clazz,
                                                          dxfg_symbol_t* pSymbolType,
                                                          int64_t fromTime, int64_t toTime)
  {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getTimeSeriesIfSubscribed";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/Object;Ljava/lang/String;)Lcom/dxfeed/api/NativeEventsList;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    const char* className = getEventClassType(clazz);
    jclass jEventTypeClass = safeFindClass(env, className);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbolType);
    jobject jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClass,
                                                            jSymbol, fromTime, toTime);

    auto* resultEventList = new dxfg_event_type_list();
    if (jNativeEventsList) {
      NativeEventsList list {env};
      list.toNativeEventsList(jNativeEventsList, &resultEventList);
    }

    env->DeleteLocalRef(jNativeEventsList);
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedJniClazz);
    return resultEventList;
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_t eventType) {
    return new dxfeed::DxSubscription(env, dxFeed_, eventType, false);
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventClasses) {
    return new dxfeed::DxSubscription(env, dxFeed_, eventClasses, false);
  }

  DxTimeSeriesSubscription* DxFeed::createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_t eventClasses) {
    return new dxfeed::DxSubscription(env, dxFeed_, eventClasses, true);
  }

  DxTimeSeriesSubscription* DxFeed::createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType) {
    return new dxfeed::DxSubscription(env, dxFeed_, eventType, true);
  }

  void DxFeed::attachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "attachSubscription";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    jmethodID methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    env->CallVoidMethod(dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  void DxFeed::detachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "detachSubscription";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    jmethodID methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    env->CallVoidMethod(dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  void DxFeed::detachSubscriptionAndClear(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "detachSubscriptionAndClear";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    jmethodID methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    env->CallVoidMethod(dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
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
