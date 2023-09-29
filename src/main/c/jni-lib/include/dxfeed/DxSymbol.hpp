// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_
#define DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {

  struct DxSymbol {
    static dxfg_symbol_t* createNativeSymbol(const char* symbol, dxfg_symbol_type_t symbolType);
    static void release(JNIEnv* env, dxfg_symbol_t* pSymbol);
    static jobject toJavaObject(JNIEnv* env, dxfg_symbol_t* pSymbolType);
    static dxfg_symbol_t* fromJavaObject(JNIEnv* env, jobject pSymbol);

  private:
    DxSymbol() = default;
    static jobject toWildcardSymbol(JNIEnv* env);
    static jobject toCandleSymbol(JNIEnv* env, const char* symbol);
    static jobject toTimeSeriesSubscriptionSymbol(JNIEnv* env, jobject symbol, jlong fromTime);
    static jobject toIndexedEventSubscriptionSymbol(JNIEnv* env, jobject symbol, jobject indexedEventSource);

    static dxfg_string_symbol_t* fromStringSymbol(JNIEnv* env, jobject jSymbol);
    static dxfg_candle_symbol_t* fromCandleSymbol(JNIEnv* env, jobject jSymbol, jclass candleSymbolClass);
    static dxfg_wildcard_symbol_t* fromWildcardSymbol(JNIEnv* env, jobject jSymbol);
    static dxfg_time_series_subscription_symbol_t* fromTimeSeriesSubscriptionSymbol(JNIEnv* env, jobject jSymbol,
                                                                                    jclass timeSeriesSubClass);
    static dxfg_indexed_event_subscription_symbol_t* fromIndexedEventSubscriptionSymbol(JNIEnv* env, jobject jSymbol,
                                                                                        jclass indexedEventSubClass);

    // allocated new memory, for const char*, need to be deallocated manually
    static const char* jStringToUTF8(JNIEnv* env, jstring jString);
    static const char* copy(const char* str);

    constexpr static const char DX_SYMBOL_JNI_CLASS_NAME[] = "com/dxfeed/api/DxSymbolJni";
    constexpr static const char STRING_SYMBOL_JNI_CLASS_NAME[] = "java/lang/String";
    constexpr static const char WILDCARD_SYMBOL_JNI_CLASS_NAME[] = "com/dxfeed/api/osub/WildcardSymbol";
    constexpr static const char CANDLE_SYMBOL_JNI_CLASS_NAME[] = "com/dxfeed/event/candle/CandleSymbol";
    constexpr static const char TIME_SERIES_SYMBOL_JNI_CLASS_NAME[] =
      "com/dxfeed/api/osub/TimeSeriesSubscriptionSymbol";
    constexpr static const char INDEXED_EVENT_SYMBOL_JNI_CLASS_NAME[] =
      "com/dxfeed/api/osub/IndexedEventSubscriptionSymbol";
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_
