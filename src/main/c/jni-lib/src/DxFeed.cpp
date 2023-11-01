// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEventT.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/DxSymbol.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/NativeEventsList.hpp"
#include "dxfeed/utils/UserDataSync.hpp"
#include "api/dxfg_api.h"

namespace dxfeed {
  using namespace jni;

  // from "dxfeed/utils/UserDataSync.hpp"
  namespace user_data_sync {
    std::mutex LOCK;
    std::condition_variable CONDITION_VAR;
    std::atomic_bool CONSUMER_PROCESSED_DATA {false };
    std::atomic_bool DATA_IS_READY { false };
    std::atomic_bool NEED_TO_EXIT { false };
    jlong GLOBAL_JAVA_USER_CALLBACK_ADDRESS = 0;
    jlong GLOBAL_JAVA_USER_DATA_ADDRESS = 0;
    std::vector<char> GLOBAL_BYTE_ARRAY;
    std::vector<double> GLOBAL_DOUBLE_ARRAY;
    std::vector<char> GLOBAL_EVENT_TYPE_ARRAY;
  }

  void consumer() {
    using namespace user_data_sync;

    JNIEnv* env = internal::javaVM->getCurrenThread();

    while (true) {
      std::unique_lock<std::mutex> locker(LOCK);

//      std::cout << "Consumer : Sleeping now\n" << std::endl;
      CONDITION_VAR.wait(locker);
      if (user_data_sync::NEED_TO_EXIT.load()) {
        return;
      }
//      std::cout << "Consumer : Got notified. Now waking up.\n" << std::endl;
      int32_t size = GLOBAL_EVENT_TYPE_ARRAY.size();
      dxfeed::jni::ByteReader reader(size, GLOBAL_BYTE_ARRAY.data(), GLOBAL_DOUBLE_ARRAY.data(), GLOBAL_EVENT_TYPE_ARRAY.data());
      auto events = reader.toEvents();
      GLOBAL_BYTE_ARRAY.clear();
      GLOBAL_DOUBLE_ARRAY.clear();
      GLOBAL_EVENT_TYPE_ARRAY.clear();

      auto pListener = dxfeed::r_cast<dxfg_feed_event_listener_function>(GLOBAL_JAVA_USER_CALLBACK_ADDRESS);
      void* userData = dxfeed::r_cast<void*>(GLOBAL_JAVA_USER_DATA_ADDRESS);
      dxfg_event_type_list list = {size, events.data()};
      pListener(env, &list, userData);
      for (const auto& event: events) {
        delete event;
      }

      locker.unlock(); // Unlock after consumption and user callback invocation.
      CONSUMER_PROCESSED_DATA.store(true);
    }
  }


  DxFeed::DxFeed(JNIEnv* env, jobject dxFeed) :
      dxFeed_(env->NewGlobalRef(dxFeed))
  {
    consumer_thread = std::thread(consumer);
  }

  DxFeed::~DxFeed() {
    internal::jniEnv->DeleteGlobalRef(dxFeed_);
    consumer_thread.join();
  }

  dxfg_feed_t* DxFeed::getInstance(JNIEnv* env) {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_CLASS_NAME);
    const char* methodName = "getInstance";
    const char* methodSignature = "()Lcom/dxfeed/api/DXFeed;";
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);
    auto jDxFeedObject = checkedCallStaticObjectMethod(env, jDxFeedJniClazz, methodId);
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
    auto jNativeEventsList = checkedCallStaticObjectMethod(env, jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClazz,
                                                           jSymbol);
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
    dxfg_event_type_list list { 1, &pEventType };
    getLastEvents(env, &list);
  }

  void DxFeed::getLastEvents(JNIEnv* env, dxfg_event_type_list* pList) {
    auto jDxFeedJniClazz = safeFindClass(env, DX_FEED_JNI_CLASS_NAME);
    const char* methodName = "getLastEvents";
    const char* methodSignature =
      "(Lcom/dxfeed/api/DXFeed;[Ljava/lang/Class;[Ljava/lang/String;)Lcom/dxfeed/api/NativeEventsList;";
    auto methodId = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);

    auto size = pList->size;
    auto jClazzClass = safeFindClass(env, "java/lang/Class");
    auto jClassArray = env->NewObjectArray(size, jClazzClass, nullptr);
    auto jStringClass = safeFindClass(env, "java/lang/String");
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

    auto jNativeEventsList = checkedCallStaticObjectMethod(env, jDxFeedJniClazz, methodId, dxFeed_, jClassArray,
                                                           jStringArray);
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
    auto jNativeEventsList = checkedCallStaticObjectMethod(env, jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClass,
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
    auto jNativeEventsList = checkedCallStaticObjectMethod(env, jDxFeedJniClazz, methodId, dxFeed_, jEventTypeClass,
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
    return dxfeed::DxSubscription::create(env, dxFeed_, eventType);
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventClasses) {
    return dxfeed::DxSubscription::create(env, dxFeed_, eventClasses);
  }

  DxTimeSeriesSubscription* DxFeed::createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_t eventClasses) {
    return dxfeed::DxSubscription::createTimeSeries(env, dxFeed_, eventClasses);
  }

  DxTimeSeriesSubscription* DxFeed::createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType) {
    return dxfeed::DxSubscription::createTimeSeries(env, dxFeed_, eventType);
  }

  void DxFeed::attachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "attachSubscription";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    auto methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  void DxFeed::detachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "detachSubscription";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    auto methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  void DxFeed::detachSubscriptionAndClear(JNIEnv* env, dxfg_subscription_t* pSubscription) {
    auto jDxFeedClass = env->GetObjectClass(dxFeed_);
    const char* methodName = "detachSubscriptionAndClear";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)V;";
    auto methodId = safeGetMethodID(env, jDxFeedClass, methodName, methodSignature);
    checkedCallVoidMethod(env, dxFeed_, methodId, pSubscription);
    env->DeleteLocalRef(jDxFeedClass);
  }

  const char* getEventClassType(dxfg_event_clazz_t eventTypeClazz) {
    switch (eventTypeClazz) {
      case DXFG_EVENT_QUOTE:
        return "com/dxfeed/event/market/Quote";
      case DXFG_EVENT_PROFILE:
        return "com/dxfeed/event/market/Profile";
      case DXFG_EVENT_SUMMARY:
        return "com/dxfeed/event/market/Summary";
      case DXFG_EVENT_GREEKS:
        return "com/dxfeed/event/option/Greeks";
      case DXFG_EVENT_CANDLE:
        return "com/dxfeed/event/candle/Candle";
      case DXFG_EVENT_DAILY_CANDLE:
        return "com/dxfeed/event/candle/DailyCandle";
      case DXFG_EVENT_UNDERLYING:
        return "com/dxfeed/event/option/Underlying";
      case DXFG_EVENT_THEO_PRICE:
        return "com/dxfeed/event/option/TheoPrice";
      case DXFG_EVENT_TRADE:
        return "com/dxfeed/event/market/Trade";
      case DXFG_EVENT_TRADE_ETH:
        return "com/dxfeed/event/market/TradeETH";
      case DXFG_EVENT_CONFIGURATION:
        return "com/dxfeed/event/misc/Configuration";
      case DXFG_EVENT_MESSAGE:
        return "com/dxfeed/event/misc/Message";
      case DXFG_EVENT_TIME_AND_SALE:
        return "com/dxfeed/event/market/TimeAndSale";
      case DXFG_EVENT_OPTION_SALE:
        return "com/dxfeed/event/market/OptionSale";
      case DXFG_EVENT_ORDER_BASE:
        return "com/dxfeed/event/market/OrderBase";
      case DXFG_EVENT_ORDER:
        return "com/dxfeed/event/market/Order";
      case DXFG_EVENT_ANALYTIC_ORDER:
        return "com/dxfeed/event/market/AnalyticOrder";
      case DXFG_EVENT_SPREAD_ORDER:
        return "com/dxfeed/event/market/SpreadOrder";
      case DXFG_EVENT_SERIES:
        return "com/dxfeed/event/option/Series";
      default:
        return nullptr;
    }
  }
}
