// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEventListener::DxEventListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxEventListener* DxEventListener::create(JNIEnv* env, dxfg_feed_event_listener_function callback, void* userData) {
    jclass pJclass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
    jmethodID newEventListenerId = jni::safeGetStaticMethodID(env, pJclass, "newEventListener", "(JJ)J");
    jlong result = env->CallStaticLongMethod(pJclass, newEventListenerId, callback, userData);
    env->DeleteLocalRef(pJclass);
    return new DxEventListener(result);
  }
}