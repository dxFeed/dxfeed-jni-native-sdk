// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXFEED_H_
#define DXFEED_JNI_NATIVE_SDK_DXFEED_H_

#include "DxSubscription.hpp"
#include "api/dxfg_events.h"

namespace dxfeed {
  struct DxFeed {
    DxFeed(JNIEnv* env, jobject obj);
    ~DxFeed();

    DxFeed(const DxFeed& other) = delete;
    DxFeed(DxFeed&& other) = delete;
    DxFeed& operator=(const DxFeed& other) = delete;
    DxFeed& operator=(DxFeed&& other) = delete;

    DxSubscription* createSubscription(dxfg_event_clazz_t eventType);
    DxSubscription* createSubscription(dxfg_event_clazz_list_t* eventType);
  private:
    jobject dxFeed_ = nullptr;
    JNIEnv* env_ = nullptr;
  };
}


#endif //DXFEED_JNI_NATIVE_SDK_DXFEED_H_
