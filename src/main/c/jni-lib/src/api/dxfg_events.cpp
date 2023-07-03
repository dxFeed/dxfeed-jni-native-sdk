// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_events.h"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxEventT.hpp"
#include "dxfeed/DxIndexedEventSource.hpp"

dxfg_symbol_t* dxfg_Symbol_new(graal_isolatethread_t* env, const char* symbol, dxfg_symbol_type_t symbolType) {
  bool isTimeSeries = (symbolType == dxfg_symbol_type_t::TIME_SERIES_SUBSCRIPTION);
  bool isIndexedEvent = (symbolType == dxfg_symbol_type_t::INDEXED_EVENT_SUBSCRIPTION);
  if (isTimeSeries || isIndexedEvent) {
    dxfeed::jni::javaLogger->error("Unknown symbol type: %", symbolType);
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
    dxfeed::jni::javaLogger->error("Unknown symbol type: ", symbolType);
    return nullptr;
  }
}

int32_t dxfg_Symbol_release(graal_isolatethread_t* env, dxfg_symbol_t* symbol) {
  delete symbol; // todo: dealloc symbol->symbol?
  return JNI_OK;
}

dxfg_event_type_t* dxfg_EventType_new(graal_isolatethread_t* env, const char* symbolName,
                                      dxfg_event_clazz_t eventTypeClazz)
{
  dxfg_event_type_t eventType {eventTypeClazz};
  return dxfeed::r_cast<dxfg_event_type_t*>(new dxfeed::DxEventT(eventType, symbolName));
}

int32_t dxfg_EventType_release(graal_isolatethread_t* env, dxfg_event_type_t* eventType) {
  auto DxEventT = dxfeed::r_cast<dxfeed::DxEventT*>(eventType);
  delete DxEventT;
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
  return dxfeed::DxIndexedEventSource::createByEventType(env, eventType);
}

