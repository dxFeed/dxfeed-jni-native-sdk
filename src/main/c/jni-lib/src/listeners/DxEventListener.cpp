// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEventListener::DxEventListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxEventListener* DxEventListener::create(JNIEnv* env, dxfg_feed_event_listener_function callback, void* userData) {
    auto dxSubscriptionJniClass = jni::safeFindClass(env, "com/dxfeed/api/DxSubscriptionJni");
    auto newEventListenerId = jni::safeGetStaticMethodID(env, dxSubscriptionJniClass, "newEventListener", "(JJ)J");
    auto result = jni::checkedCallStaticLongMethod(env, dxSubscriptionJniClass, newEventListenerId, callback, userData);
    env->DeleteLocalRef(dxSubscriptionJniClass);
    return new DxEventListener(result);
  }
}