// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_events.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxLastingEvent.hpp"

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
