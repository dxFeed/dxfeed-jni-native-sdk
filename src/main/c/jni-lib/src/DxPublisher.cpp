// SPDX-License-Identifier: MPL-2.0

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
    auto jDxFeedObject = env->CallStaticObjectMethod(jDxPublisherClazz, methodId);
    env->DeleteLocalRef(jDxPublisherClazz);
    return dxfeed::r_cast<dxfg_publisher_t*>(new DxPublisher(env, jDxFeedObject));
  }

  int32_t DxPublisher::publishEvents(JNIEnv* env, dxfg_event_type_list* events) {
    auto jDxPublisherClazz = env->GetObjectClass(dxPublisher_);
    const char* methodName = "publishEvents";
    const char* methodSignature = "(Ljava/util/Collection;)V";
    auto methodId = safeGetMethodID(env, jDxPublisherClazz, methodName, methodSignature);
    NativeEventsList list {env};
    auto nativeEventsList = list.fromNativeEventsList(events);;
    env->CallVoidMethod(dxPublisher_, methodId, nativeEventsList);
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
    auto jDxSubscription = env->CallObjectMethod(dxPublisher_, methodId, jEventTypeClass);
    auto subscription = dxfeed::DxSubscription::createObservable(env, jDxSubscription);
    env->DeleteLocalRef(jDxSubscription);
    env->DeleteLocalRef(jEventTypeClass);
    env->DeleteLocalRef(jDxPublisherClazz);
    return subscription;
  }
}
