// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed {
  DxFeed::DxFeed(JNIEnv* env, jobject dxFeed) :
      dxFeed_(env->NewGlobalRef(dxFeed))
  {}

  DxFeed::~DxFeed() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(dxFeed_);
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_t eventType) {
    return new DxSubscription(env, dxFeed_, eventType);
  }

  DxSubscription* DxFeed::createSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventClazzes) {
    return new DxSubscription(env, dxFeed_, eventClazzes);
  }
}
