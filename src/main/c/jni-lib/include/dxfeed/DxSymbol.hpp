// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_
#define DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {

  struct DxSymbol {
    static jobject toJavaObject(JNIEnv* env, dxfg_symbol_t* pSymbolType);
    static dxfg_symbol_t* fromJavaObject(JNIEnv* env, jobject pSymbol);
  private:
    DxSymbol() = default;
    static jobject toWildcardSymbol(JNIEnv* env);
    static jobject toCandleSymbol(JNIEnv* env, const char* symbol);
    static jobject toTimeSeriesSubscriptionSymbol(JNIEnv* env, jobject symbol, jlong fromTime);
    static jobject toIndexedEventSubscriptionSymbol(JNIEnv* env, jobject symbol, jobject indexedEventSource);

    constexpr static const char DX_SYMBOL_JNI_CLASS_NAME[] = "Lcom/dxfeed/api/DxSymbolJni;";
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXSYMBOL_H_
