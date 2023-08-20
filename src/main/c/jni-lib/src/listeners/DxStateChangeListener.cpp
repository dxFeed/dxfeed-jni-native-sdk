// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxStateChangeListener::DxStateChangeListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxStateChangeListener* DxStateChangeListener::create(JNIEnv* env, dxfg_endpoint_state_change_listener_func callback,
                                                       void* userData)
  {
    auto dxEndpointClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxEndpointJni;");
    auto newStateChangeListenerId =
      jni::safeGetStaticMethodID(env, dxEndpointClass, "newStateChangeEventListener", "(JJ)J");
    jlong result = env->CallStaticLongMethod(dxEndpointClass, newStateChangeListenerId, callback, userData);
    env->DeleteLocalRef(dxEndpointClass);
    return new DxStateChangeListener(result);
  }
}