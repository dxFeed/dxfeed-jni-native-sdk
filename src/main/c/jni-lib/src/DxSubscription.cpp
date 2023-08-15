// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed {
  using namespace jni;

  DxSubscription::DxSubscription(JNIEnv* env, dxfg_event_clazz_t eventType) {
    auto jDxSubscriptionClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_CLASS_NAME);
    const char* className = getEventClassType(eventType);
    jclass jEventTypeClass = safeFindClass(env, className);
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, "<init>", "(Ljava/lang/Class;)V");
    jobject jDxSubscription = env->NewObject(jDxSubscriptionClass, methodId, jEventTypeClass);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, dxfg_event_clazz_t, bool), jDxSubscription: %", subscription_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxSubscriptionClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventClasses) {
    int32_t size = eventClasses->size;
    jclass jClazzArrayClass = safeFindClass(env, "Ljava/lang/Class;");
    jobjectArray jArray = env->NewObjectArray(size, jClazzArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClasses->elements[i];
      const char* className = getEventClassType(*pClazz);
      jclass jEventTypeClass = safeFindClass(env, className);
      env->SetObjectArrayElement(jArray, i, jEventTypeClass);
      env->DeleteLocalRef(jEventTypeClass);
    }

    auto jDxSubscriptionClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_CLASS_NAME);
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, "<init>", "([Ljava/lang/Class;)V");
    jobject jDxSubscription = env->NewObject(jDxSubscriptionClass, methodId, jArray);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, dxfg_event_clazz_list_t*, bool), jDxSubscription: %", subscription_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jClazzArrayClass);
    env->DeleteLocalRef(jDxSubscriptionClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_t eventType, bool isTimeSeries) {
    jclass jDxFeedClass = env->GetObjectClass(dxFeed);
    const char* className = getEventClassType(eventType);
    jclass jEventTypeClass = safeFindClass(env, className);
    jmethodID methodId = getMethodId(env, jDxFeedClass, isTimeSeries, false);
    jobject jDxSubscription = env->CallObjectMethod(dxFeed, methodId, jEventTypeClass);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, jobject, dxfg_event_clazz_t, bool), jDxSubscription: %", subscription_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxFeedClass);
  }

  DxSubscription::DxSubscription(JNIEnv* env, jobject dxFeed, dxfg_event_clazz_list_t* eventClasses, bool isTimeSeries) {
    int32_t size = eventClasses->size;
    jclass jClazzArrayClass = safeFindClass(env, "Ljava/lang/Class;");
    jobjectArray jArray = env->NewObjectArray(size, jClazzArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_event_clazz_t* pClazz = eventClasses->elements[i];
      const char* className = getEventClassType(*pClazz);
      jclass jEventTypeClass = safeFindClass(env, className);
      env->SetObjectArrayElement(jArray, i, jEventTypeClass);
      env->DeleteLocalRef(jEventTypeClass);
    }
    jclass jDxFeedClass = env->GetObjectClass(dxFeed);
    jmethodID methodId = getMethodId(env, jDxFeedClass, isTimeSeries, true);
    jobject jDxSubscription = env->CallObjectMethod(dxFeed, methodId, jArray);
    subscription_ = env->NewGlobalRef(jDxSubscription);
    javaLogger->info("DxSubscription(JNIEnv*, jobject, dxfg_event_clazz_list_t*, bool), jDxSubscription: %",
                     subscription_);

    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jArray);
    env->DeleteLocalRef(jClazzArrayClass);
    env->DeleteLocalRef(jDxFeedClass);
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

  DxSubscription::~DxSubscription() {
    internal::jniEnv->DeleteGlobalRef(subscription_);
  }

  void DxSubscription::addListener(JNIEnv* env, DxEventListener* listener) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "addEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);
    env->CallStaticVoidMethod(jDxSubscriptionJniClass, methodId, subscription_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxSubscriptionJniClass);
  }

  void DxSubscription::removeListener(JNIEnv* env, DxEventListener* listener) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "removeEventListener";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;J)V";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);
    env->CallStaticVoidMethod(jDxSubscriptionJniClass, methodId, subscription_, listener->javaListenerId_);
    env->DeleteLocalRef(jDxSubscriptionJniClass);
  }

  int32_t DxSubscription::addSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "addSymbols";
    const char* methodSignature = "(Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = safeFindClass(env, "Ljava/lang/Object;");
    jobjectArray jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "addSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName,methodSignature);
    env->CallVoidMethod(subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::setSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::setSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = safeFindClass(env, "Ljava/lang/Object;");
    jobjectArray jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  void DxSubscription::close(JNIEnv* env) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "close";
    const char* methodSignature = "()V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId);
    env->DeleteLocalRef(jDxSubscriptionClass);
  }

  int32_t DxSubscription::setTime(graal_isolatethread_t* env, int64_t time) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "setTime";
    const char* methodSignature = "(J)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, time);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::removeSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "removeSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    jobject jSymbol = DxSymbol::toJavaObject(env, pSymbol);
    if (jSymbol) {
      env->CallVoidMethod(subscription_, methodId, jSymbol);
    }
    env->DeleteLocalRef(jSymbol);
    return jSymbol ? JNI_OK : JNI_ERR;
  }

  int32_t DxSubscription::removeSymbols(JNIEnv* env, dxfg_symbol_list* symbols) {
    int32_t size = symbols->size;
    jclass jObjectArrayClass = safeFindClass(env, "Ljava/lang/Object;");
    jobjectArray jSymbolsArray = env->NewObjectArray(size, jObjectArrayClass, nullptr);
    for (int i = 0; i < size; ++i) {
      dxfg_symbol_t* symbol = symbols->elements[i];
      jobject jSymbol = DxSymbol::toJavaObject(env, symbol);
      env->SetObjectArrayElement(jSymbolsArray, i, jSymbol);
    }

    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "removeSymbols";
    const char* methodSignature = "([Ljava/lang/Object;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, jSymbolsArray);
    env->DeleteLocalRef(jObjectArrayClass);
    env->DeleteLocalRef(jSymbolsArray);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::attach(JNIEnv* env, DxFeed* pFeed) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "attach";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeed;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, pFeed->getJavaObject());
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::detach(JNIEnv* env, DxFeed* pFeed) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "detach";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeed;)V";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    env->CallVoidMethod(subscription_, methodId, pFeed->getJavaObject());
    env->DeleteLocalRef(jDxSubscriptionClass);
    return JNI_OK;
  }

  int32_t DxSubscription::isClosed(JNIEnv* env) {
    auto jDxSubscriptionClass = env->GetObjectClass(subscription_);
    const char* methodName = "isClosed";
    const char* methodSignature = "()Z";
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    jboolean result = env->CallBooleanMethod(subscription_, methodId);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return result ? JNI_OK : JNI_ERR;
  }

  dxfg_event_clazz_list_t* DxSubscription::getEventTypes(JNIEnv* env) {
    auto jDxSubscriptionJniClass = safeFindClass(env, DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME);
    const char* methodName = "getEventTypes";
    const char* methodSignature = "(Lcom/dxfeed/api/DXFeedSubscription;)[Z";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSubscriptionJniClass, methodName, methodSignature);

    auto jByteArray = r_cast<jbyteArray>(env->CallStaticObjectMethod(jDxSubscriptionJniClass, methodId, subscription_));
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
    jclass jEventTypeClazz = safeFindClass(env, className);
    jmethodID methodId = safeGetMethodID(env, jDxSubscriptionClass, methodName, methodSignature);
    jboolean result = env->CallBooleanMethod(subscription_, methodId, jEventTypeClazz);

    env->DeleteLocalRef(jEventTypeClazz);
    env->DeleteLocalRef(jDxSubscriptionClass);
    return result ? JNI_OK : JNI_ERR;
  }

  dxfg_symbol_list* DxSubscription::getSymbols(JNIEnv* env) {
    const char* methodName = "getSymbols";
    const char* methodSignature = nullptr;
    // todo: finish
    return nullptr;
  }

}