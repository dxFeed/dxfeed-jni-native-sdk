// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEventListener::DxEventListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxEventListener* DxEventListener::create(JNIEnv* env, dxfg_feed_event_listener_function callback, void* userData) {
    auto dxSubscriptionJniClass = dxfeed::jni::internal::dxJni->dxSubscriptionJniClass_;
    jmethodID newEventListenerId = jni::safeGetStaticMethodID(env, dxSubscriptionJniClass, "newEventListener", "(JJ)J");
    jlong result = env->CallStaticLongMethod(dxSubscriptionJniClass, newEventListenerId, callback, userData);
    return new DxEventListener(result);
  }
}