// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_DXFEED_H_
#define DXFEED_JNI_NATIVE_SDK_DXFEED_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {
  const char* getEventClassType(dxfg_event_clazz_t eventTypeClazz);
  struct DxSubscription;
  typedef struct DxSubscription DxTimeSeriesSubscription;
  typedef struct DxSubscription DxObservableSubscription;

  struct DxFeed {
    DxFeed(JNIEnv* env, jobject obj);
    ~DxFeed();

    DxFeed(const DxFeed& other) = delete;
    DxFeed(DxFeed&& other) = delete;
    DxFeed& operator=(const DxFeed& other) = delete;
    DxFeed& operator=(DxFeed&& other) = delete;

    static dxfg_feed_t* getInstance(JNIEnv* env);
    jobject getJavaObject() const;
    DxSubscription* createSubscription(JNIEnv* env, dxfg_event_clazz_t eventType);
    DxSubscription* createSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType);
    DxTimeSeriesSubscription* createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_t eventType);
    DxTimeSeriesSubscription* createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType);
    void attachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription);
    void detachSubscription(JNIEnv* env, dxfg_subscription_t* pSubscription);
    void detachSubscriptionAndClear(JNIEnv* env, dxfg_subscription_t* pSubscription);

    void getLastEvent(JNIEnv* env, dxfg_event_type_t* pType);
    void getLastEvents(JNIEnv* env, dxfg_event_type_list* pList);
    dxfg_event_type_t* getLastEventIfSubscribed(JNIEnv* env, dxfg_event_clazz_t clazz, dxfg_symbol_t* pSymbol);
    dxfg_event_type_list* getIndexedEventsIfSubscribed(JNIEnv* env, dxfg_event_clazz_t clazz, dxfg_symbol_t* pSymbol,
                                                       const char* string);
    dxfg_event_type_list* getTimeSeriesIfSubscribed(JNIEnv* env, dxfg_event_clazz_t clazz, dxfg_symbol_t* pSymbol,
                                                    int64_t fromTime, int64_t toTime);

  private:
    jobject dxFeed_ = nullptr;

    constexpr static const char DX_FEED_CLASS_NAME[] = "com/dxfeed/api/DXFeed";
    constexpr static const char DX_FEED_JNI_CLASS_NAME[] = "com/dxfeed/api/DxFeedJni";
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXFEED_H_
