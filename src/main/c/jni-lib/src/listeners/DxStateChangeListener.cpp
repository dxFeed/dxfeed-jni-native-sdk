// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxStateChangeListener::DxStateChangeListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxStateChangeListener* DxStateChangeListener::create(JNIEnv* env, dxfg_endpoint_state_change_listener_func callback,
                                                       void* userData)
  {
    jclass clazz = dxfeed::jni::internal::dxJni->dxEndpointJniClass_;
    jmethodID newStateChangeListenerId = jni::safeGetStaticMethodID(env, clazz, "newStateChangeEventListener", "(JJ)J");
    jlong result = env->CallStaticLongMethod(clazz, newStateChangeListenerId, callback, userData);
    return new DxStateChangeListener(result);
  }
}