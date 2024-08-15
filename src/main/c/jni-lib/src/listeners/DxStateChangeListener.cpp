// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
DxStateChangeListener::DxStateChangeListener(jlong listenerId) : javaListenerId_(listenerId) {
}

DxStateChangeListener *DxStateChangeListener::create(JNIEnv *env, dxfg_endpoint_state_change_listener_func callback,
                                                     void *userData) {
    auto dxEndpointClass = jni::safeFindClass(env, "com/dxfeed/api/DxEndpointJni");
    auto newStateChangeListenerId =
        jni::safeGetStaticMethodID(env, dxEndpointClass, "newStateChangeEventListener", "(JJ)J");
    auto pCallback = dxfeed::r_cast<jlong>(callback);
    auto pUserData = dxfeed::r_cast<jlong>(userData);
    auto result =
        jni::checkedCallStaticLongMethod(env, dxEndpointClass, newStateChangeListenerId, pCallback, pUserData);
    env->DeleteLocalRef(dxEndpointClass);
    return new DxStateChangeListener(result);
}
} // namespace dxfeed