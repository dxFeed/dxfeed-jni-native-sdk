// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/DxSymbol.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/UserDataSync.hpp"

namespace dxfeed {
  using namespace jni;

  // todo: make user_data_sync non-global, one per subscription
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
      auto size = static_cast<int32_t>(GLOBAL_EVENT_TYPE_ARRAY.size());
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

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxSubscription):
    subscription_(env->NewGlobalRef(dxSubscription))
  {
    consumer_thread = std::thread(consumer);
  }

  DxSubscription::~DxSubscription() {
//    std::cout << "~DxSubscription" << std::endl;
    user_data_sync::NEED_TO_EXIT.store(true);
    user_data_sync::CONDITION_VAR.notify_one();
    consumer_thread.join();
//    std::cout << "~consumer_thread joined!" << std::endl;
    internal::jniEnv->DeleteGlobalRef(subscription_);
  }

  DxSubscription* DxSubscription::create(JNIEnv* env, dxfg_event_clazz_t eventType) {
    auto jDxSubscriptionClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_CLASS_NAME);
    const char* className = getEventClassType(eventType);
    auto jEventTypeClass = safeFindClass(env, className);
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, "<init>", "(Ljava/lang/Class;)V");
    auto jDxSubscription = env->NewObject(jDxSubscriptionClass, methodId, jEventTypeClass);
    javaLogger->trace(env, "create(JNIEnv*, dxfg_event_clazz_t), jDxSubscription: %", jDxSubscription);

    auto result = new DxSubscription(env, jDxSubscription);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return result;
  }

  DxSubscription* DxSubscription::create(JNIEnv* env, jobject connection,
                                         dxfg_event_clazz_t eventType, bool isTimeSeries)
  {
    auto jDxFeedClass = env->GetObjectClass(connection);
    const char* className = getEventClassType(eventType);
    auto jEventTypeClass = safeFindClass(env, className);
    auto methodId = getMethodId(env, jDxFeedClass, isTimeSeries, false);
    auto jDxSubscription = checkedCallObjectMethod(env, connection, methodId, jEventTypeClass);
    javaLogger->trace(env, "create(jobject, dxfg_event_clazz_t, bool), jDxSubscription: %", jDxSubscription);

    auto result = new DxSubscription(env, jDxSubscription);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedClass);
    return result;
  }

  DxSubscription* DxSubscription::create(JNIEnv* env, dxfg_event_clazz_list_t* eventClasses) {
    auto jArray = buildJavaObjectArray(env, eventClasses);
    auto jDxSubscriptionClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_CLASS_NAME);
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, "<init>", "([Ljava/lang/Class;)V");
    auto jDxSubscription = env->NewObject(jDxSubscriptionClass, methodId, jArray);
    javaLogger->trace(env, "create(dxfg_event_clazz_list_t*), jDxSubscription: %", jDxSubscription);

    auto result = new DxSubscription(env, jDxSubscription);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return result;
  }

  DxSubscription* DxSubscription::create(JNIEnv* env, jobject conn, dxfg_event_clazz_list_t* eventClasses,
                                         bool isTimeSeries)
  {
    auto jArray = buildJavaObjectArray(env, eventClasses);
    auto jDxFeedClass = env->GetObjectClass(conn);
    auto methodId = getMethodId(env, jDxFeedClass, isTimeSeries, true);
    auto jDxSubscription = checkedCallObjectMethod(env, conn, methodId, jArray);
    javaLogger->trace(env, "create(jobject, dxfg_event_clazz_list_t*, bool), jDxSubscription: %",
                     jDxSubscription);

    auto result = new DxSubscription(env, jDxSubscription);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jDxFeedClass);

    return result;
  }

  DxSubscription* DxSubscription::create(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType) {
    return create(env, connection, eventType, false);
  }

  DxSubscription* DxSubscription::create(JNIEnv* env, jobject connection, dxfg_event_clazz_list_t* eventClasses) {
    return create(env, connection, eventClasses, false);
  }

  DxSubscription* DxSubscription::createTimeSeries(JNIEnv* env, jobject conn, dxfg_event_clazz_t eventType) {
    return create(env, conn, eventType, true);
  }

  DxSubscription* DxSubscription::createTimeSeries(JNIEnv* env, jobject conn, dxfg_event_clazz_list_t* eventClasses) {
    return create(env, conn, eventClasses, true);
  }

  DxObservableSubscription* DxSubscription::createObservable(JNIEnv* env, jobject observableSubscription) {
    return new DxSubscription(env, observableSubscription);
  }

  void DxSubscription::addListener(JNIEnv* env, DxEventListener* listener) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "addEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    auto methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);
    checkedCallStaticVoidMethod(env, jDxSubscriptionJniClass, methodId, subscription_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxSubscriptionJniClass);
  }

  void DxSubscription::removeListener(JNIEnv* env, DxEventListener* listener) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "removeEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    auto methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);
    checkedCallStaticVoidMethod(env, jDxSubscriptionJniClass, methodId, subscription_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxSubscriptionJniClass);
  }

  int32_t DxSubscription::addSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "addSymbols";
    const char* methodSignature = "(Ljava/lang/Object;)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    auto jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      checkedCallVoidMethod(env, subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    auto jObjectArrayClass = safeFindClass(env, "java/lang/Object");
    auto jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      auto jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "addSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName,methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::setSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    dxfg_symbol_list l {1, &pSymbol};
    return setSymbols(env, &l);
  }

  int32_t DxSubscription::setSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    auto jObjectArrayClass = safeFindClass(env, "java/lang/Object");
    auto jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      auto jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  void DxSubscription::close(JNIEnv* env) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "close";
    const char* methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId);
    env->DeleteLocalRef(jDxSubscriptionClass);
  }

  int32_t DxSubscription::setTime(graal_isolatethread_t* env, int64_t time) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setTime";
    const char* methodSignature = "(J)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId, time);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::removeSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    dxfg_symbol_list l {1, &pSymbol};
    return removeSymbols(env, &l);
  }

  int32_t DxSubscription::removeSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    auto jObjectArrayClass = safeFindClass(env, "java/lang/Object");
    auto jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      auto jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "removeSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::clear(JNIEnv* env) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "clear";
    const char* methodSignature = "()V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::attach(JNIEnv* env, DxFeed* pFeed) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "attach";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeed;)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId, pFeed->getJavaObject());
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::detach(JNIEnv* env, DxFeed* pFeed) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "detach";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeed;)V";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    checkedCallVoidMethod(env, subscription_, methodId, pFeed->getJavaObject());
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::isClosed(JNIEnv* env) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "isClosed";
    const char* methodSignature = "()Z";
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    auto result = env->CallBooleanMethod(subscription_, methodId);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return result ? JNI_OK : JNI_ERR;
  }

  dxfg_event_clazz_list_t* DxSubscription::getEventTypes(JNIEnv* env) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "getEventTypes";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)[Z";
    auto methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);

    auto jByteArray = checkedCallStaticObjectMethodAndCastTo<jbyteArray>(env, jDxSubscriptionJniClass, methodId,
                                                                         subscription_);
    jint size = env->GetArrayLength(jByteArray);
    auto* pEventTypeData = r_cast<char*>(env->GetPrimitiveArrayCritical(jByteArray, 0));

    auto result = new dxfg_event_clazz_list_t();
    result->size = size;
    result->elements = new dxfg_event_clazz_t*[size];
    for (int i = 0; i < size; ++i) {
      auto* pClazz = new dxfg_event_clazz_t { static_cast<dxfg_event_clazz_t>(pEventTypeData[i]) };
      result->elements[i] = pClazz;
    }

    env->ReleasePrimitiveArrayCritical(jByteArray, pEventTypeData, 0);
    env->DeleteLocalRef(jByteArray);
    env->DeleteLocalRef(jDxSubscriptionJniClass);

    return result;
  }

  int32_t DxSubscription::containsEventType(JNIEnv* env, dxfg_event_clazz_t eventTypeClass) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "containsEventType";
    const char* methodSignature = "(Ljava/lang/Class;)Z";
    const char* className = getEventClassType(eventTypeClass);
    auto jEventTypeClazz = safeFindClass(env, className);
    auto methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    jboolean result = env->CallBooleanMethod(subscription_, methodId, jEventTypeClazz);

    env->DeleteLocalRef(jEventTypeClazz);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return result ? JNI_OK : JNI_ERR;
  }

  dxfg_symbol_list* DxSubscription::getSymbols(JNIEnv* env) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "getSymbols";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)[java/lang/Object;";
    auto methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);
    auto jArray = r_cast<jobjectArray>(env->CallStaticObjectMethod(jDxSubscriptionJniClass, methodId, subscription_));
    jint size = env->GetArrayLength(jArray);

    auto list = new dxfg_symbol_list();
    list->size = size;
    list->elements = new dxfg_symbol_t*[size];
    for (int i = 0; i < size; ++i) {
      /*  https://developer.ibm.com/articles/j-jni/
       *  Each time GetObjectArrayElement() is called, a local reference is created for the element and isn't freed
       *  until the native completes. The larger the array, the more local references will be created.
       *
       *  These local references are freed automatically when the native method terminates.
       *  The JNI specification requires that each native be able to create at least 16 local references.
       *  Although this is adequate for many methods, some methods need to access more during their lifetime.
       *  In this case, you should either delete references that are no longer needed, by using the
       *  JNI DeleteLocalRef() call, or inform the JVM that you'll be using a larger number of local references.
       * */
      auto jObject = env->GetObjectArrayElement(jArray, i);
      list->elements[i] = DxSymbol::fromJavaObject(env, jObject);
      env->DeleteLocalRef(jObject);
    }

    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jDxSubscriptionJniClass);

    return list;
  }

  dxfg_symbol_list* DxSubscription::getDecoratedSymbols(JNIEnv* env) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "getDecoratedSymbols";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)[java/lang/Object;";
    auto methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);
    auto jArray = r_cast<jobjectArray>(env->CallStaticObjectMethod(jDxSubscriptionJniClass, methodId, subscription_));
    jint size = env->GetArrayLength(jArray);

    auto list = new dxfg_symbol_list();
    list->size = size;
    list->elements = new dxfg_symbol_t*[size];
    for (int i = 0; i < size; ++i) {
      /*  https://developer.ibm.com/articles/j-jni/
       *  Each time GetObjectArrayElement() is called, a local reference is created for the element and isn't freed
       *  until the native completes. The larger the array, the more local references will be created.
       *
       *  These local references are freed automatically when the native method terminates.
       *  The JNI specification requires that each native be able to create at least 16 local references.
       *  Although this is adequate for many methods, some methods need to access more during their lifetime.
       *  In this case, you should either delete references that are no longer needed, by using the
       *  JNI DeleteLocalRef() call, or inform the JVM that you'll be using a larger number of local references.
       * */
      auto jObject = env->GetObjectArrayElement(jArray, i);
      list->elements[i] = DxSymbol::fromJavaObject(env, jObject);
      env->DeleteLocalRef(jObject);
    }

    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jDxSubscriptionJniClass);

    return list;
  }

  jobjectArray DxSubscription::buildJavaObjectArray(JNIEnv* env, const dxfg_event_clazz_list_t* eventClasses) {
    int32_t size = eventClasses->size;
    auto jClazzArrayClass = safeFindClass(env, "java/lang/Class");
    auto jArray = env->NewObjectArray(size, jClazzArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClasses->elements[i];
      const char* className = getEventClassType(*pClazz);
      auto jEventTypeClass = safeFindClass(env, className);
      env->SetObjectArrayElement(jArray, i, jEventTypeClass);
      env->DeleteLocalRef(jEventTypeClass);
    }
    env->DeleteLocalRef(jClazzArrayClass);
    return jArray;
  }

  jmethodID DxSubscription::getMethodId(JNIEnv* env, jclass clazz, bool isTimeSeries, bool argIsArray) {
    const char* methodName;
    const char* signature;
    if (isTimeSeries) {
      methodName = "createTimeSeriesSubscription";
      signature = argIsArray  ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;"
                              : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedTimeSeriesSubscription;";
    } else {
      methodName = "createSubscription";
      signature = argIsArray  ? "([Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;"
                              : "(Ljava/lang/Class;)Lcom/dxfeed/api/DXFeedSubscription;";
    }
    return safeGetMethodID(env, clazz, methodName, signature);
  }

}