// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
DxEventListener::DxEventListener(jlong listenerId) : javaListenerId_(listenerId) {
}

DxEventListener *DxEventListener::create(JNIEnv *env, dxfg_feed_event_listener_function callback, void *userData) {
    auto dxSubscriptionJniClass = jni::safeFindClass(env, "com/dxfeed/api/DxSubscriptionJni");
    auto newEventListenerId = jni::safeGetStaticMethodID(env, dxSubscriptionJniClass, "newEventListener", "(JJ)J");
    auto pCallback = dxfeed::r_cast<jlong>(callback);
    auto pUserData = dxfeed::r_cast<jlong>(userData);
    auto result =
        jni::checkedCallStaticLongMethod(env, dxSubscriptionJniClass, newEventListenerId, pCallback, pUserData);
    env->DeleteLocalRef(dxSubscriptionJniClass);
    return new DxEventListener(result);
}
} // namespace dxfeed