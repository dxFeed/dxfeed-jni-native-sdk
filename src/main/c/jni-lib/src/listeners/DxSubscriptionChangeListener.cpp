// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxSubscriptionChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
DxSubscriptionChangeListener::DxSubscriptionChangeListener(jlong listenerId) : javaListenerId_(listenerId) {
}

DxSubscriptionChangeListener *DxSubscriptionChangeListener::create(
    JNIEnv *env, dxfg_ObservableSubscriptionChangeListener_function_symbolsAdded fSymbolsAdded,
    dxfg_ObservableSubscriptionChangeListener_function_symbolsRemoved fSymbolsRemoved,
    dxfg_ObservableSubscriptionChangeListener_function_subscriptionClosed fSubscriptionClosed, void *userData) {
    auto dxEndpointClass = jni::safeFindClass(env, "com/dxfeed/api/DxSubscriptionJni");
    auto newChangeListenerId = jni::safeGetStaticMethodID(env, dxEndpointClass, "newChangeListener", "(JJJJ)J");
    auto pOnSymbolsAdded = dxfeed::r_cast<jlong>(fSymbolsAdded);
    auto pOnSymbolsRemoved = dxfeed::r_cast<jlong>(fSymbolsRemoved);
    auto pOnSubscriptionClosed = dxfeed::r_cast<jlong>(fSubscriptionClosed);
    auto pUserData = dxfeed::r_cast<jlong>(userData);
    auto result = jni::checkedCallStaticLongMethod(env, dxEndpointClass, newChangeListenerId, pOnSymbolsAdded,
                                                   pOnSymbolsRemoved, pOnSubscriptionClosed, pUserData);
    env->DeleteLocalRef(dxEndpointClass);
    return new DxSubscriptionChangeListener(result);
}
} // namespace dxfeed