// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  using namespace jni::internal;

  DxEventListener::DxEventListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxEventListener* DxEventListener::create(JNIEnv* env, dxfg_feed_event_listener_function callback, void* userData) {
    jmethodID newEventListenerId = jni::safeGetStaticMethodID(env, dxFeedJniClass->clazz, "newEventListener", "(JJ)J");
    jlong result = env->CallStaticLongMethod(dxFeedJniClass->clazz, newEventListenerId, callback, userData);
    return new DxEventListener(result);
  }
}