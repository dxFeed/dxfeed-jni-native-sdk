// SPDX-License-Identifier: MPL-2.0


#include "api/dxfg_events.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_event_type_t* dxfg_EventType_new(graal_isolatethread_t* env, const char* symbolName, dxfg_event_clazz_t eventTypeClazz) {
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
    return reinterpret_cast<dxfg_event_type_t*>(result); // todo: or use DxLastingEvent wrapper?
}

int32_t dxfg_EventType_release(graal_isolatethread_t* env, dxfg_event_type_t* eventType) {
    jclass dxFeedJniClass = dxfeed::jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
    jmethodID methodId = dxfeed::jni::safeGetStaticMethodID(env, dxFeedJniClass, "releaseEvent","(j)V");
    env->CallStaticVoidMethod(dxFeedJniClass, methodId, reinterpret_cast<jlong>(eventType));
    env->DeleteLocalRef(dxFeedJniClass);
    return JNI_OK;
}
