// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxSymbol.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxIndexedEventSource.hpp"

namespace dxfeed {
  using namespace jni;

  // return JNI local references which will be invalid after passing them to VM
  jobject DxSymbol::toJavaObject(JNIEnv* env, dxfg_symbol_t* pSymbolType) {
    switch (pSymbolType->type) {
      case STRING: {
        auto pStringSymbol = r_cast<dxfg_string_symbol_t*>(pSymbolType);
        return env->NewStringUTF(pStringSymbol->symbol);
      }
      case WILDCARD: {
        return toWildcardSymbol(env);
      }
      case CANDLE: {
        auto pCandleSymbol = r_cast<dxfg_candle_symbol_t*>(pSymbolType);
        return toCandleSymbol(env, pCandleSymbol->symbol);
      }
      case TIME_SERIES_SUBSCRIPTION: {
        auto pTssSymbol = r_cast<dxfg_time_series_subscription_symbol_t*>(pSymbolType);
        jobject symbol = toJavaObject(env, pTssSymbol->symbol);
        return toTimeSeriesSubscriptionSymbol(env, symbol, pTssSymbol->from_time);
      }
      case INDEXED_EVENT_SUBSCRIPTION: {
        auto pIesSymbol = r_cast<dxfg_indexed_event_subscription_symbol_t*>(pSymbolType);
        jobject symbol = toJavaObject(env, pIesSymbol->symbol);
        jobject indexedEventSource = r_cast<DxIndexedEventSource*>(pIesSymbol->source)->javaObject(); // todo: delete Global Ref ?
        return toIndexedEventSubscriptionSymbol(env, symbol, indexedEventSource);
      }
      default:
        return nullptr;
    } 
  }

  jobject DxSymbol::toWildcardSymbol(JNIEnv* env) {
    jclass jDxSymbolJniClass = internal::dxJni->dxSymbolJniClass_;
    const char* methodName = "newWildCardSymbol";
    const char* methodSignature = "()Lcom/dxfeed/api/osub/WildcardSymbol;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    jobject jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  jobject DxSymbol::toCandleSymbol(JNIEnv* env, const char* symbol) {
    jclass jDxSymbolJniClass = internal::dxJni->dxSymbolJniClass_;
    const char* methodName = "newCandleSymbol";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/event/candle/CandleSymbol;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    jstring jSymbolName = env->NewStringUTF(symbol);
    jobject jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId, jSymbolName);
    env->DeleteLocalRef(jSymbolName);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  jobject DxSymbol::toTimeSeriesSubscriptionSymbol(JNIEnv* env, jobject symbol, jlong fromTime) {
    jclass jDxSymbolJniClass = internal::dxJni->dxSymbolJniClass_;
    const char* methodName = "newTimeSeriesSubscriptionSymbol";
    const char* methodSignature = "(Ljava/lang/Object;J)Lcom/dxfeed/api/osub/TimeSeriesSubscriptionSymbol;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    jobject jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId, symbol, fromTime);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  jobject DxSymbol::toIndexedEventSubscriptionSymbol(JNIEnv* env, jobject symbol, jobject indexedEventSource) {
    jclass jDxSymbolJniClass = internal::dxJni->dxSymbolJniClass_;
    const char* methodName = "newIndexedEventSubscriptionSymbol";
    const char* methodSignature =
        "(Ljava/lang/Object;Lcom/dxfeed/event/IndexedEventSource;)Lcom/dxfeed/api/osub/IndexedEventSubscriptionSymbol;";
    jmethodID methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    jobject jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId, symbol, indexedEventSource);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }
}
