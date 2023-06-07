// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxEventListener::DxEventListener(JNIEnv* env, dxfg_feed_event_listener_function userFunc, void* userData) {
    jclass pJclass = jni::safeFindClass(env, "Lcom/dxfeed/api/JNIDXFeedEventListener;");
    jmethodID newEventListenerId = jni::safeGetStaticMethodID(env, pJclass, "newEventListener",
                                                              "(J)Lcom/dxfeed/api/JNIDXFeedEventListener;");
    eventListener_ = env->NewGlobalRef(
        env->CallStaticObjectMethod(pJclass, newEventListenerId, reinterpret_cast<jlong>(this))
    );
    env->DeleteLocalRef(pJclass);
    userFunc_ = userFunc;
    userData_ = userData;
  }

  DxEventListener::~DxEventListener() {
    userFunc_ = nullptr;
    userData_ = nullptr;
  }

  jobject DxEventListener::getJavaHandle() const {
    return eventListener_;
  }

  void DxEventListener::callUserFunc(graal_isolatethread_t* thread, dxfg_event_type_list* events) {
    userFunc_(thread, events, userData_);
  }

  void DxEventListener::removeJavaRef() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(eventListener_);
  }
}