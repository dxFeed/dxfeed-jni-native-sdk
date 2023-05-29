// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  DxStateChangeListener::DxStateChangeListener(JNIEnv* env, dxfg_endpoint_state_change_listener_func userFunc,
                                               void* userData)
  {
    jclass pJclass = jni::safeFindClass(env, "Lcom/dxfeed/api/JNIPropertyChangeListener;");
    jmethodID newStateChangeListenerId = jni::safeGetStaticMethodID(env, pJclass, "newStateChangeEventListener",
                                                                    "(J)Lcom/dxfeed/api/JNIPropertyChangeListener;");
    stateChangeListener_ = env->NewGlobalRef(
      env->CallStaticObjectMethod(pJclass, newStateChangeListenerId, reinterpret_cast<jlong>(this))
    );
    env->DeleteLocalRef(pJclass);
    userFunc_ = userFunc;
    userData_ = userData;
  }

  DxStateChangeListener::~DxStateChangeListener() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(stateChangeListener_);
  }

  jobject DxStateChangeListener::getJavaHandle() const {
    return stateChangeListener_;
  }

  void DxStateChangeListener::callUserFunc(graal_isolatethread_t* thread, int32_t oldStateEnum, int32_t newStateEnum) {
    auto oldState = static_cast<dxfg_endpoint_state_t>(oldStateEnum);
    auto newState = static_cast<dxfg_endpoint_state_t>(newStateEnum);
    userFunc_(thread, oldState, newState, userData_);
  }

  void DxStateChangeListener::clear() {
    userFunc_ = nullptr;
    userData_ = nullptr;
  }
}