// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DxPublisher.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/NativeEventsList.hpp"

namespace dxfeed {
  using namespace jni;

  DxPublisher::DxPublisher(JNIEnv* env, jobject dxPublisher) :
    dxPublisher_(env->NewGlobalRef(dxPublisher))
  {}

  DxPublisher::~DxPublisher() {
    internal::jniEnv->DeleteGlobalRef(dxPublisher_);
  }

  dxfg_publisher_t* DxPublisher::getInstance(JNIEnv* env) {
    auto jDxPublisherClazz = safeFindClass(env, DX_PUBLISHER_CLASS_NAME);
    const char* methodName = "getInstance";
    const char* methodSignature = "()Lcom/dxfeed/api/DXPublisher;";
    auto methodId = safeGetStaticMethodID(env, jDxPublisherClazz, methodName, methodSignature);
    auto jDxFeedObject = checkedCallStaticObjectMethod(env, jDxPublisherClazz, methodId);
    env->DeleteLocalRef(jDxPublisherClazz);
    return dxfeed::r_cast<dxfg_publisher_t*>(new DxPublisher(env, jDxFeedObject));
  }

  int32_t DxPublisher::publishEvents(JNIEnv* env, dxfg_event_type_list* events) {
    auto jDxPublisherClazz = env->GetObjectClass(dxPublisher_);
    const char* methodName = "publishEvents";
    const char* methodSignature = "(Ljava/util/Collection;)V";
    auto methodId = safeGetMethodID(env, jDxPublisherClazz, methodName, methodSignature);
    auto nativeEventsList = NativeEventsList::fromNativeEventsList(env, events);
    checkedCallVoidMethod(env, dxPublisher_, methodId, nativeEventsList);
    env->DeleteLocalRef(jDxPublisherClazz);
    return JNI_OK;
  }

  DxObservableSubscription* DxPublisher::getSubscription(JNIEnv* env, dxfg_event_clazz_t eventType) {
    auto jDxPublisherClazz = env->GetObjectClass(dxPublisher_);
    const char* className = getEventClassType(eventType);
    auto jEventTypeClass = safeFindClass(env, className);
    const char* methodName = "getSubscription";
    const char* methodSignature = "(Ljava/lang/Class;)Lcom/dxfeed/api/osub/ObservableSubscription;";
    auto methodId = safeGetMethodID(env, jDxPublisherClazz, methodName, methodSignature);
    auto jDxSubscription = checkedCallObjectMethod(env, dxPublisher_, methodId, jEventTypeClass);
    auto subscription = dxfeed::DxSubscription::createObservable(env, jDxSubscription);
    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxPublisherClazz);
    return subscription;
  }
}
