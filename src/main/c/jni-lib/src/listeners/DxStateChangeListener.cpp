// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxStateChangeListener::DxStateChangeListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxStateChangeListener* DxStateChangeListener::create(JNIEnv* env, dxfg_endpoint_state_change_listener_func callback,
                                                       void* userData)
  {
    jclass pJclass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni;");
    jmethodID newStateChangeListenerId = jni::safeGetStaticMethodID(env, pJclass, "newStateChangeEventListener",
                                                                    "(JJ)J");
    jlong result = env->CallStaticLongMethod(pJclass, newStateChangeListenerId, callback, userData);
    env->DeleteLocalRef(pJclass);
    return new DxStateChangeListener(result);
  }
}