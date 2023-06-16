// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_events.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxLastingEvent.hpp"
#include "dxfeed/DxSymol.hpp"

dxfg_symbol_t* dxfg_Symbol_new(graal_isolatethread_t* env, const char* symbol, dxfg_symbol_type_t symbolType) {
  jclass dxFeedJniClass = dxfeed::jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxFeedJniClass, "newSymbol",
                                                          "(Ljava/lang/String;I)J");
  jlong result = env->CallStaticLongMethod(dxFeedJniClass, methodId, symbolType);
  auto dxSymbol = new dxfeed::DxSymbol(symbolType, result);
  return reinterpret_cast<dxfg_symbol_t*>(dxSymbol);
}

int32_t dxfg_Symbol_release(graal_isolatethread_t* env, dxfg_symbol_t* symbol) {
  jclass dxFeedJniClass = dxfeed::jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxFeedJniClass, "releaseSymbol","(J)V");
  auto dxSymbol = reinterpret_cast<dxfeed::DxSymbol*>(symbol);
  env->CallStaticVoidMethod(dxFeedJniClass, methodId, dxSymbol->nativeHandlerId);
  delete dxSymbol;
  return JNI_OK;
}

dxfg_event_type_t* dxfg_EventType_new(graal_isolatethread_t* env, const char* symbolName,
                                      dxfg_event_clazz_t eventTypeClazz)
{
  const char* className = dxfeed::getEventClassType(eventTypeClazz);
  jclass eventTypeClass = dxfeed::jni::safeFindClass(env, className);
  jstring jSymbolName = env->NewStringUTF(symbolName);

  jclass dxFeedJniClass = dxfeed::jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxFeedJniClass, "newEvent",
                                                          "(Ljava/lang/Class;Ljava/lang/String;)J");
  jlong result = env->CallStaticLongMethod(dxFeedJniClass, methodId, eventTypeClass, jSymbolName);

  env->DeleteLocalRef(dxFeedJniClass);
  env->DeleteLocalRef(jSymbolName);
  env->DeleteLocalRef(eventTypeClass);

  dxfg_event_type_t eventType {eventTypeClazz};
  return reinterpret_cast<dxfg_event_type_t*>(new dxfeed::DxLastingEvent(eventType, result));
}

int32_t dxfg_EventType_release(graal_isolatethread_t* env, dxfg_event_type_t* eventType) {
  jclass dxFeedJniClass = dxfeed::jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
  jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxFeedJniClass, "releaseEvent", "(J)V");
  auto dxLastingEvent = reinterpret_cast<dxfeed::DxLastingEvent*>(eventType);
  env->CallStaticVoidMethod(dxFeedJniClass, methodId, dxLastingEvent->eventType);
  env->DeleteLocalRef(dxFeedJniClass);
  delete dxLastingEvent;
  return JNI_OK;
}
