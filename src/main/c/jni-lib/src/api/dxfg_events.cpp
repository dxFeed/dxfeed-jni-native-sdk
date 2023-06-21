// SPDX-License-Identifier: MPL-2.0

#include <sstream>
#include <iostream>

#include "api/dxfg_events.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxLastingEvent.hpp"
#include "dxfeed/DxIndexedEventSource.hpp"

using namespace dxfeed::jni::internal;

// todo: what's wrong with TIME_SERIES_SUBSCRIPTION and INDEXED_EVENT_SUBSCRIPTION ??
dxfg_symbol_t* dxfg_Symbol_new(graal_isolatethread_t* env, const char* symbol, dxfg_symbol_type_t symbolType) {
  bool isTimeSeries = (symbolType == dxfg_symbol_type_t::TIME_SERIES_SUBSCRIPTION);
  bool isIndexedEvent = (symbolType == dxfg_symbol_type_t::INDEXED_EVENT_SUBSCRIPTION);
  if (isTimeSeries || isIndexedEvent) {
    std::cerr << "Unknown symbol type: " + std::to_string(symbolType);
    return nullptr;
  }
  if (symbolType == dxfg_symbol_type_t::STRING) {
    auto result = new dxfg_string_symbol_t();
    result->supper = dxfg_symbol_t { symbolType };
    result->symbol = symbol; // todo: alloc & copy?
    return dxfeed::r_cast<dxfg_symbol_t*>(result);
  } else if (symbolType ==  dxfg_symbol_type_t::CANDLE) {
    auto result = new dxfg_candle_symbol_t();
    result->supper = dxfg_symbol_t { symbolType };
    result->symbol = symbol; // todo: alloc & copy?
    return dxfeed::r_cast<dxfg_symbol_t*>(result);
  } else if (symbolType ==  dxfg_symbol_type_t::WILDCARD) {
    auto result = new dxfg_wildcard_symbol_t();
    result->supper = dxfg_symbol_t { symbolType };
    return dxfeed::r_cast<dxfg_symbol_t*>(result);
  } else {
    std::cerr << "Unknown symbol type: " + std::to_string(symbolType);
    return nullptr;
  }
}

int32_t dxfg_Symbol_release(graal_isolatethread_t* env, dxfg_symbol_t* symbol) {
  delete symbol; // todo: dealloc symbol->symbol?
  return JNI_OK;
}

// todo: don't create Java object here? Crate it before JNI call to VM
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
  return dxfeed::r_cast<dxfg_indexed_event_source_t*>(new dxfeed::DxIndexedEventSource(env, source));
}

int32_t dxfg_IndexedEventSource_release(graal_isolatethread_t* env, dxfg_indexed_event_source_t* source) {
  auto dxIndexedEventSource = dxfeed::r_cast<dxfeed::DxIndexedEventSource*>(source);
  dxIndexedEventSource->release(env);
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
      if (!dxfeed::DxIndexedEventSource::isSpecialSourceId(env, sourceId)) {
        sourceId = static_cast<int32_t>(index >> 32);
      }
      return dxfeed::r_cast<dxfg_indexed_event_source_t*>(new dxfeed::DxIndexedEventSource(env, sourceId));
    }
    default: {
      std::stringstream ss{};
      const char* className = dxfeed::getEventClassType(eventType->clazz);
      ss << "ClassCastException(" << className << " is not Class<? extends IndexedEvent>";
      std::cerr << "Unknown symbol type: " + ss.str();
      return nullptr;
    }
  }
}

