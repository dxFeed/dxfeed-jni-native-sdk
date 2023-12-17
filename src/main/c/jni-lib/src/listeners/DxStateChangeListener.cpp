// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxStateChangeListener::DxStateChangeListener(jlong listenerId): javaListenerId_(listenerId) {}

  DxStateChangeListener* DxStateChangeListener::create(JNIEnv* env, dxfg_endpoint_state_change_listener_func callback,
                                                       void* userData)
  {
    auto dxEndpointClass = jni::safeFindClass(env, "com/dxfeed/api/DxEndpointJni");
    auto newStateChangeListenerId =
      jni::safeGetStaticMethodID(env, dxEndpointClass, "newStateChangeEventListener", "(JJ)J");
    auto result = jni::checkedCallStaticLongMethod(env, dxEndpointClass, newStateChangeListenerId, callback, userData);
    env->DeleteLocalRef(dxEndpointClass);
    return new DxStateChangeListener(result);
  }
}