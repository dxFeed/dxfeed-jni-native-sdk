// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXFEED_H_
#define DXFEED_JNI_NATIVE_SDK_DXFEED_H_

#include <jni.h>

#include "api/dxfg_events.h"
#include "api/dxfg_feed.h"

namespace dxfeed {
  const char* getEventClassType(dxfg_event_type_t eventType);
  const char* getEventClassType(dxfg_event_clazz_t eventTypeClazz);
  struct DxSubscription;
  typedef struct DxSubscription DxTimeSeriesSubscription;

  struct DxFeed {
    DxFeed(JNIEnv* env, jobject obj);
    ~DxFeed();

    DxFeed(const DxFeed& other) = delete;
    DxFeed(DxFeed&& other) = delete;
    DxFeed& operator=(const DxFeed& other) = delete;
    DxFeed& operator=(DxFeed&& other) = delete;

    static dxfg_feed_t* getInstance(JNIEnv* env);
    DxSubscription* createSubscription(JNIEnv* env, dxfg_event_clazz_t eventType);
    DxSubscription* createSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType);
    DxTimeSeriesSubscription* createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_t eventType);
    DxTimeSeriesSubscription* createTimeSeriesSubscription(JNIEnv* env, dxfg_event_clazz_list_t* eventType);
    void attachSubscription(graal_isolatethread_t* pEnv, dxfg_subscription_t* pSubscription);
    void detachSubscription(graal_isolatethread_t* pEnv, dxfg_subscription_t* pSubscription);
    void detachSubscriptionAndClear(graal_isolatethread_t* pEnv, dxfg_subscription_t* pSubscription);

    dxfg_event_type_t* getLastEventIfSubscribed(graal_isolatethread_t* env, dxfg_event_clazz_t clazz, dxfg_symbol_t* pSymbol);
    void getLastEvent(graal_isolatethread_t* env, dxfg_event_type_t* pType);
    void getLastEvents(graal_isolatethread_t* env, dxfg_event_type_list* pList);

  private:
    jobject dxFeed_ = nullptr;
    jclass dxFeedClass_ = nullptr;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXFEED_H_
