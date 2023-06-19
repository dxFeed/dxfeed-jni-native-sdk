// SPDX-License-Identifier: MPL-2.0

#include <sstream>

#include "api/dxfg_events.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxLastingEvent.hpp"
#include "dxfeed/DxSymol.hpp"
#include "dxfeed/DxOrderSource.hpp"

using namespace dxfeed::jni::internal;

dxfg_symbol_t* dxfg_Symbol_new(graal_isolatethread_t* env, const char* symbol, dxfg_symbol_type_t symbolType) {
  auto dxSymbolClass = dxJni->dxSymbolJniClass_;
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxSymbolClass, "newSymbol", "(Ljava/lang/String;I)J");
  jstring jSymbol = env->NewStringUTF(symbol);
  jlong result = env->CallStaticLongMethod(dxSymbolClass, methodId, jSymbol, symbolType);
  env->DeleteLocalRef(jSymbol);
  auto dxSymbol = new dxfeed::DxSymbol(symbolType, result);
  return dxfeed::r_cast<dxfg_symbol_t*>(dxSymbol);
}

int32_t dxfg_Symbol_release(graal_isolatethread_t* env, dxfg_symbol_t* symbol) {
  auto dxSymbolClass = dxJni->dxSymbolJniClass_;
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxSymbolClass, "releaseSymbol", "(J)V");
  auto dxSymbol = dxfeed::r_cast<dxfeed::DxSymbol*>(symbol);
  env->CallStaticVoidMethod(dxSymbolClass, methodId, dxSymbol->nativeHandlerId);
  delete dxSymbol;
  return JNI_OK;
}

dxfg_event_type_t* dxfg_EventType_new(graal_isolatethread_t* env, const char* symbolName,
                                      dxfg_event_clazz_t eventTypeClazz)
{
  const char* className = dxfeed::getEventClassType(eventTypeClazz);
  jclass eventTypeClass = dxfeed::jni::safeFindClass(env, className);
  jstring jSymbolName = env->NewStringUTF(symbolName);

  auto dxFeedClass = dxJni->dxFeedJniClass_;
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxFeedClass, "newEvent", "(Ljava/lang/Class;"
                                                                                        "Ljava/lang/String;)J");
  jlong result = env->CallStaticLongMethod(dxFeedClass, methodId, eventTypeClass, jSymbolName);

  env->DeleteLocalRef(jSymbolName);
  env->DeleteLocalRef(eventTypeClass);

  dxfg_event_type_t eventType {eventTypeClazz};
  return dxfeed::r_cast<dxfg_event_type_t*>(new dxfeed::DxLastingEvent(eventType, result));
}

int32_t dxfg_EventType_release(graal_isolatethread_t* env, dxfg_event_type_t* eventType) {
  jclass clazz = dxJni->dxFeedJniClass_;
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, clazz, "releaseEvent", "(J)V");
  auto dxLastingEvent = dxfeed::r_cast<dxfeed::DxLastingEvent*>(eventType);
  env->CallStaticVoidMethod(clazz, methodId, dxLastingEvent->eventType);
  delete dxLastingEvent;
  return JNI_OK;
}

dxfg_indexed_event_source_t* dxfg_IndexedEventSource_new(graal_isolatethread_t* env, const char* source) {
  return dxfeed::r_cast<dxfg_indexed_event_source_t*>(new dxfeed::DxOrderSource(env, source));
}

int32_t dxfg_IndexedEventSource_release(graal_isolatethread_t* env, dxfg_indexed_event_source_t* source) {
  auto dxIndexedEventSource = dxfeed::r_cast<dxfeed::DxOrderSource*>(source);
  dxIndexedEventSource->releaseJavaObject(env);
  delete dxIndexedEventSource;
  return JNI_OK;
}

dxfg_indexed_event_source_t* dxfg_IndexedEvent_getSource(graal_isolatethread_t* env, dxfg_event_type_t* eventType) {
  switch (eventType->clazz) {
    case DXFG_EVENT_GREEKS:
    case DXFG_EVENT_CANDLE:
    case DXFG_EVENT_DAILY_CANDLE:
    case DXFG_EVENT_TIME_AND_SALE:
    case DXFG_EVENT_UNDERLYING:
    case DXFG_EVENT_THEO_PRICE:
    case DXFG_EVENT_SERIES: {
      return dxfg_IndexedEventSource_new(env, nullptr);
    }
    case DXFG_EVENT_ORDER_BASE:
    case DXFG_EVENT_SPREAD_ORDER:
    case DXFG_EVENT_ORDER:
    case DXFG_EVENT_ANALYTIC_ORDER: {
      const auto index = dxfeed::r_cast<dxfg_order_base_t*>(eventType)->index;
      int sourceId = static_cast<int32_t>(index >> 48);
      if (!dxfeed::DxOrderSource::isSpecialSourceId(env, sourceId)) {
        sourceId = static_cast<int32_t>(index >> 32);
      }
      return dxfeed::DxOrderSource::createOrderSourceById(env, sourceId);
    }
    default: {
      std::stringstream ss{};
      const char* className = dxfeed::getEventClassType(eventType->clazz);
      ss << "ClassCastException(" << className << " is not Class<? extends IndexedEvent>";
      throw std::runtime_error(ss.str());
    }
  }
}

