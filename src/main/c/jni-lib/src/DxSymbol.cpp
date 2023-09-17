// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxSymbol.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxIndexedEventSource.hpp"

namespace dxfeed {
  using namespace jni;

  void DxSymbol::release(JNIEnv * env, dxfg_symbol_t* pSymbol) {
    if (!pSymbol) {
      return;
    }
    dxfg_symbol_type_t symbolType = pSymbol->type;
    switch (symbolType) {
      case dxfg_symbol_type_t::STRING: {
        auto stringSymbol = r_cast<dxfg_string_symbol_t*>(pSymbol);
        delete stringSymbol->symbol;
        return;
      }
      case dxfg_symbol_type_t::CANDLE: {
        auto candleSymbol = r_cast<dxfg_candle_symbol_t*>(pSymbol);
        delete candleSymbol->symbol;
        return;
      }
      case dxfg_symbol_type_t::WILDCARD: {
        return;
      }
      case dxfg_symbol_type_t::TIME_SERIES_SUBSCRIPTION: {
        auto timeSeriesSymbol = r_cast<dxfg_time_series_subscription_symbol_t*>(pSymbol);
        release(env, timeSeriesSymbol->symbol);
      }
      case dxfg_symbol_type_t::INDEXED_EVENT_SUBSCRIPTION: {
        auto indexedEventSymbol = r_cast<dxfg_indexed_event_subscription_symbol_t*>(pSymbol);
        auto pSource = r_cast<DxIndexedEventSource*>(indexedEventSymbol->source);
        delete pSource;
        release(env, indexedEventSymbol->symbol);
      }
      default: {
        javaLogger->error("Unknown symbol type: ", symbolType);
      }
    }
  }

  dxfg_symbol_t* DxSymbol::createNativeSymbol(const char* symbol, dxfg_symbol_type_t symbolType) {
    bool isTimeSeries = (symbolType == dxfg_symbol_type_t::TIME_SERIES_SUBSCRIPTION);
    bool isIndexedEvent = (symbolType == dxfg_symbol_type_t::INDEXED_EVENT_SUBSCRIPTION);
    if (isTimeSeries || isIndexedEvent) {
      javaLogger->error("Unknown symbol type: %", symbolType);
      return nullptr;
    }
    switch (symbolType) {
      case dxfg_symbol_type_t::STRING: {
        auto result = new dxfg_string_symbol_t();
        result->supper = dxfg_symbol_t { symbolType };
        result->symbol = copy(symbol);
        return dxfeed::r_cast<dxfg_symbol_t*>(result);
      }
      case dxfg_symbol_type_t::CANDLE: {
        auto result = new dxfg_candle_symbol_t();
        result->supper = dxfg_symbol_t { symbolType };
        result->symbol = copy(symbol);
        return dxfeed::r_cast<dxfg_symbol_t*>(result);
      }
      case dxfg_symbol_type_t::WILDCARD: {
        auto result = new dxfg_wildcard_symbol_t();
        result->supper = dxfg_symbol_t { symbolType };
        return dxfeed::r_cast<dxfg_symbol_t*>(result);
      }
      default: {
        javaLogger->error("Unknown symbol type: ", symbolType);
        return nullptr;
      }
    }
  }

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
        auto symbol = toJavaObject(env, pTssSymbol->symbol);
        return toTimeSeriesSubscriptionSymbol(env, symbol, pTssSymbol->from_time);
      }
      case INDEXED_EVENT_SUBSCRIPTION: {
        auto pIesSymbol = r_cast<dxfg_indexed_event_subscription_symbol_t*>(pSymbolType);
        auto symbol = toJavaObject(env, pIesSymbol->symbol);
        auto indexedEventSource = r_cast<DxIndexedEventSource*>(pIesSymbol->source)->javaObject();
        return toIndexedEventSubscriptionSymbol(env, symbol, indexedEventSource);
      }
      default:
        return nullptr;
    } 
  }

  jobject DxSymbol::toWildcardSymbol(JNIEnv* env) {
    auto jDxSymbolJniClass = safeFindClass(env, DX_SYMBOL_JNI_CLASS_NAME);
    const char* methodName = "newWildCardSymbol";
    const char* methodSignature = "()Lcom/dxfeed/api/osub/WildcardSymbol;";
    auto methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    auto jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  jobject DxSymbol::toCandleSymbol(JNIEnv* env, const char* symbol) {
    auto jDxSymbolJniClass = safeFindClass(env, DX_SYMBOL_JNI_CLASS_NAME);
    const char* methodName = "newCandleSymbol";
    const char* methodSignature = "(Ljava/lang/String;)Lcom/dxfeed/event/candle/CandleSymbol;";
    auto methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    auto jSymbolName = env->NewStringUTF(symbol);
    auto jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId, jSymbolName);
    env->DeleteLocalRef(jSymbolName);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  jobject DxSymbol::toTimeSeriesSubscriptionSymbol(JNIEnv* env, jobject symbol, jlong fromTime) {
    auto jDxSymbolJniClass = safeFindClass(env, DX_SYMBOL_JNI_CLASS_NAME);
    const char* methodName = "newTimeSeriesSubscriptionSymbol";
    const char* methodSignature = "(Ljava/lang/Object;J)Lcom/dxfeed/api/osub/TimeSeriesSubscriptionSymbol;";
    auto methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    auto jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId, symbol, fromTime);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  jobject DxSymbol::toIndexedEventSubscriptionSymbol(JNIEnv* env, jobject symbol, jobject indexedEventSource) {
    auto jDxSymbolJniClass = safeFindClass(env, DX_SYMBOL_JNI_CLASS_NAME);
    const char* methodName = "newIndexedEventSubscriptionSymbol";
    const char* methodSignature =
        "(Ljava/lang/Object;Lcom/dxfeed/event/IndexedEventSource;)Lcom/dxfeed/api/osub/IndexedEventSubscriptionSymbol;";
    auto methodId = safeGetStaticMethodID(env, jDxSymbolJniClass, methodName, methodSignature);
    auto jSymbol = env->CallStaticObjectMethod(jDxSymbolJniClass, methodId, symbol, indexedEventSource);
    env->DeleteLocalRef(jDxSymbolJniClass);
    return jSymbol;
  }

  dxfg_symbol_t* DxSymbol::fromJavaObject(JNIEnv* env, jobject pSymbol) {
    auto stringSymbolClass = safeFindClass(env, STRING_SYMBOL_JNI_CLASS_NAME);
    auto candleSymbolClass = safeFindClass(env, CANDLE_SYMBOL_JNI_CLASS_NAME);
    auto wildcardSymbolClass = safeFindClass(env, WILDCARD_SYMBOL_JNI_CLASS_NAME);
    auto timeSeriesSymbolClass = safeFindClass(env, TIME_SERIES_SYMBOL_JNI_CLASS_NAME);
    auto indexedEventSymbolClass = safeFindClass(env, INDEXED_EVENT_SYMBOL_JNI_CLASS_NAME);

    const std::string& pSymbolName = dxfeed::jni::internal::javaLangClass->getName(env, pSymbol);
    javaLogger->error("Processing symbol type: %", pSymbolName);

    dxfg_symbol_t* result = nullptr;
    if (env->IsInstanceOf(pSymbol, stringSymbolClass)) {
      result = r_cast<dxfg_symbol_t*>(fromStringSymbol(env, pSymbol));
    } else if (env->IsInstanceOf(pSymbol, candleSymbolClass)) {
      result = r_cast <dxfg_symbol_t*>(fromCandleSymbol(env, pSymbol, candleSymbolClass));
    } else if (env->IsInstanceOf(pSymbol, wildcardSymbolClass)) {
      result = r_cast <dxfg_symbol_t*>(fromWildcardSymbol(env, pSymbol));
    } else if (env->IsInstanceOf(pSymbol, timeSeriesSymbolClass)) {
      result = r_cast <dxfg_symbol_t*>(fromTimeSeriesSubscriptionSymbol(env, pSymbol, timeSeriesSymbolClass));
    } else if (env->IsInstanceOf(pSymbol, indexedEventSymbolClass)) {
      result = r_cast <dxfg_symbol_t*>(fromIndexedEventSubscriptionSymbol(env, pSymbol, indexedEventSymbolClass));
    } else {
      javaLogger->error("Unknown symbol type: %", pSymbolName);
    }
    env->DeleteLocalRef(stringSymbolClass);
    env->DeleteLocalRef(candleSymbolClass);
    env->DeleteLocalRef(wildcardSymbolClass);
    env->DeleteLocalRef(timeSeriesSymbolClass);
    env->DeleteLocalRef(indexedEventSymbolClass);
    return result;
  }

  dxfg_string_symbol_t* DxSymbol::fromStringSymbol(JNIEnv* env, jobject jSymbol) {
    auto resultSymbol = new dxfg_string_symbol_t();
    resultSymbol->supper.type = STRING;

    auto jString = r_cast<jstring>(jSymbol);
    resultSymbol->symbol = jStringToUTF8(env, jString);
    env->DeleteLocalRef(jString);
    return resultSymbol;
  }

  dxfg_candle_symbol_t* DxSymbol::fromCandleSymbol(JNIEnv* env, jobject jSymbol, jclass candleSymbolClass) {
    auto resultSymbol = new dxfg_candle_symbol_t();
    resultSymbol->supper.type = CANDLE;

    const char* methodName = "toString";
    const char* methodSignature = "()Ljava/lang/String;";
    auto methodId = safeGetMethodID(env, candleSymbolClass, methodName, methodSignature);
    auto jString = r_cast<jstring>(env->CallStaticObjectMethod(candleSymbolClass, methodId, jSymbol));
    resultSymbol->symbol = jStringToUTF8(env, jString);
    env->DeleteLocalRef(jString);
    return resultSymbol;
  }

  dxfg_wildcard_symbol_t* DxSymbol::fromWildcardSymbol(JNIEnv* env, jobject jSymbol) {
    auto resultSymbol = new dxfg_wildcard_symbol_t();
    resultSymbol->supper.type = WILDCARD;
    return resultSymbol;
  }

  dxfg_time_series_subscription_symbol_t* DxSymbol::fromTimeSeriesSubscriptionSymbol(JNIEnv* env, jobject jSymbol,
                                                                                     jclass timeSeriesSubClass)
  {
    auto resultSymbol = new dxfg_time_series_subscription_symbol_t();
    resultSymbol->supper.type = TIME_SERIES_SUBSCRIPTION;

    const char* methodName = "getFromTime";
    const char* methodSignature = "()J";
    auto methodId = safeGetMethodID(env, timeSeriesSubClass, methodName, methodSignature);
    resultSymbol->from_time = env->CallLongMethod(jSymbol, methodId);

    methodName = "getEventSymbol";
    methodSignature = "()Ljava/lang/Object;";
    methodId = safeGetMethodID(env, timeSeriesSubClass, methodName, methodSignature);
    auto subSymbol = env->CallObjectMethod(jSymbol, methodId);
    resultSymbol->symbol = fromJavaObject(env, subSymbol);
    env->DeleteLocalRef(subSymbol);
    return resultSymbol;
  }

  dxfg_indexed_event_subscription_symbol_t* DxSymbol::fromIndexedEventSubscriptionSymbol(JNIEnv* env,
                                                                                       jobject jSymbol,
                                                                                       jclass indexedEventSubClass)
  {
    auto resultSymbol = new dxfg_indexed_event_subscription_symbol_t();
    resultSymbol->supper.type = INDEXED_EVENT_SUBSCRIPTION;
    const char* methodName = "name";
    const char* methodSignature = "()Ljava/lang/String;";
    auto methodId = safeGetMethodID(env, indexedEventSubClass, methodName, methodSignature);
    auto sourceName = r_cast<jstring>(env->CallObjectMethod(jSymbol, methodId));
    const char* utfChars = env->GetStringUTFChars(sourceName, 0);
    resultSymbol->source = dxfg_IndexedEventSource_new(env, utfChars);
    env->ReleaseStringUTFChars(sourceName, utfChars);

    resultSymbol->symbol = fromJavaObject(env, jSymbol);

    return resultSymbol;
  }

  const char* DxSymbol::jStringToUTF8(JNIEnv* env, jstring jString) {
    const char* jData = env->GetStringUTFChars(jString, 0);
    const char* copiedData = copy(jData);
    env->ReleaseStringUTFChars(jString, jData);
    return copiedData;
  }

  const char* DxSymbol::copy(const char* str) {
    auto len = static_cast<int32_t>(strlen(str));
    char* copiedData = new char[len + 1];
    copiedData[len] = 0;
    memcpy(copiedData, str, len);
    return copiedData;
  }
}
