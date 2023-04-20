// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxFeed.h"

namespace dxfeed {
  DxFeed::DxFeed(JNIEnv* env, jobject dxFeed, const dxfeed::OnCloseHandler onClose) :
      env_(env),
      dxFeed_(env->NewGlobalRef(dxFeed)),
      onClose_(onClose) {}

  DxFeed::~DxFeed() {
    onClose_(dxFeed_);
  }

  DxSubscription* DxFeed::createSubscription(dxfg_event_clazz_t eventType) {
    return new DxSubscription(env_, dxFeed_, eventType, onClose_);
  }
}
