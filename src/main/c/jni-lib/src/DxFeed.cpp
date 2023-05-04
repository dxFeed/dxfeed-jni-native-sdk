// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxFeed.hpp"

namespace dxfeed {
  DxFeed::DxFeed(JNIEnv* env, jobject dxFeed) :
      env_(env),
      dxFeed_(env->NewGlobalRef(dxFeed))
  {}

  DxFeed::~DxFeed() {
    env_->DeleteGlobalRef(dxFeed_);
  }

  DxSubscription* DxFeed::createSubscription(dxfg_event_clazz_t eventType) {
    return new DxSubscription(env_, dxFeed_, eventType);
  }

  DxSubscription* DxFeed::createSubscription(dxfg_event_clazz_list_t* eventClazzes) {
    return new DxSubscription(env_, dxFeed_, eventClazzes);
  }
}
