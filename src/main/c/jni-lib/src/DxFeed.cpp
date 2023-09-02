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
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);
    auto jDxFeedObject = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId);
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
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    const char* className = getEventClassType(eventTypeClass);
    auto jEventTypeClazz = safeFindClass(env, className);
    auto jSymbol = DxSymbol::toJavaObject(env, pSymbolType);
    auto jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClazz, jSymbol);
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

  // todo: check
  void DxFeed::getLastEvent(JNIEnv* env, dxfg_event_type_t* pEventType) {
    dxfg_event_type_list list { 1, &pEventType };
    getLastEvents(env, &list);
  }

  // todo: check
  void DxFeed::getLastEvents(JNIEnv* env, dxfg_event_type_list* pList) {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getLastEvents";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;[Ljava/lang/Class;[Ljava/lang/String;)Lcom/dxfeed/api/NativeEventsList;";
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    auto size = pList->size;
    auto jClazzClass = safeFindClass(env, "Ljava/lang/Class;");
    auto jClassArray = env->NewObjectArray(size, jClazzClass, nullptr);
    auto jStringClass = safeFindClass(env, "Ljava/lang/String;");
    auto jStringArray = env->NewObjectArray(size, jStringClass, nullptr);

    for (int i = 0; i < size; ++i) {
      dxfg_event_type_t* pEventType = pList->elements[i];
      auto dxEventT = r_cast<DxEventT*>(pEventType);
      const char* className = getEventClassType(dxEventT->eventType_.clazz);
      auto jEventTypeClass = safeFindClass(env, className);
      auto jSymbolName = env->NewStringUTF(dxEventT->symbol_);

      env->SetObjectArrayElement(jClassArray, i, jEventTypeClass);
      env->SetObjectArrayElement(jStringArray, i, jSymbolName);

      // https://forums.oracle.com/ords/apexds/post/use-deletelocalref-with-setobjectarrayelement-0632
      // https://stackoverflow.com/questions/4369974/using-deletelocalref-after-setobjectarrayelement-when-building-array-in-jni
      env->DeleteLocalRef(jEventTypeClass);
      env->DeleteLocalRef(jSymbolName);
    }

    auto jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jClassArray, jStringArray);
    if (jNativeEventsList) {
      NativeEventsList data{env};
      data.toNativeEventsList(jNativeEventsList, &pList);
    }
    env->DeleteLocalRef(jNativeEventsList);
    env->DeleteLocalRef(jStringArray);
    env->DeleteLocalRef(jStringClass);
    env->DeleteLocalRef(jClassArray);
    env->DeleteLocalRef(jClazzClass);

    env->DeleteLocalRef(jDxFeedJniClazz);
  }

  dxfg_event_type_list* DxFeed::getIndexedEventsIfSubscribed(JNIEnv* env, dxfg_event_clazz_t clazz,
                                                             dxfg_symbol_t* pSymbolType, const char* source)
  {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getIndexedEventsIfSubscribed";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;Ljava/lang/Class;Ljava/lang/Object;Ljava/lang/String;)Lcom/dxfeed/api/NativeEventsList;";
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    const char* className = getEventClassType(clazz);
    auto jEventTypeClass = safeFindClass(env, className);
    auto jSymbol = DxSymbol::toJavaObject(env, pSymbolType);
    auto jSource = env->NewStringUTF(source);
    auto jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClass,
                                                           jSymbol, jSource);

    auto* resultEventList = new dxfg_event_type_list();
    if (jNativeEventsList) {
      NativeEventsList list {env};
      list.toNativeEventsList(jNativeEventsList, &resultEventList);
    }

    env->DeleteLocalRef(jNativeEventsList);
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
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    const char* className = getEventClassType(clazz);
    auto jEventTypeClass = safeFindClass(env, className);
    auto jSymbol = DxSymbol::toJavaObject(env, pSymbolType);
    auto jNativeEventsList = env->CallStaticObjectMethod(jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClass,
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
    auto methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    env->CallVoidMethod(dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  void DxFeed::detachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "detachSubscription";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    auto methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    env->CallVoidMethod(dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  void DxFeed::detachSubscriptionAndClear(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "detachSubscriptionAndClear";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    auto methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
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
      case DXFG_EVENT_OPTION_SALE:
        return "Lcom/dxfeed/event/market/OptionSale;";
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
      default:
        return nullptr;
    }
  }
}
