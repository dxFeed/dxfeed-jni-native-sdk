// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_SDK_DXFEED_H_
#define DXFEED_JNI_SDK_DXFEED_H_

#include "Subscription.hpp"
#include "api/dxfg_events.h"

namespace dxfeed {
  struct DxFeed {
    DxFeed(JNIEnv* env, jobject obj, OnCloseHandler onClose);
    ~DxFeed();

    Subscription* createSubscription(dxfg_event_clazz_t eventType);
  private:
    jobject dxFeed_ = nullptr;
    JNIEnv* env_ = nullptr;
    const dxfeed::OnCloseHandler onClose_ = nullptr;
  };
}


#endif //DXFEED_JNI_SDK_DXFEED_H_
