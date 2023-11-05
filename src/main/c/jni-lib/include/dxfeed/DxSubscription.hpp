// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_

#include <jni.h>
#include <string>

#include "api/dxfg_events.h"
#include "DxFeed.hpp"

namespace dxfeed {
  struct DxEventListener;

  struct DxSubscription final {
    constexpr static const char JAVA_SUBSCRIPTION_CLASS_NAME[] = "com.dxfeed.api.DXFeedSubscription";
    constexpr static const char JAVA_TIME_SERIES_SUBSCRIPTION_NAME[] = "com.dxfeed.api.DXFeedTimeSeriesSubscription";

    static DxObservableSubscription* createObservable(JNIEnv* env, jobject observableSubscription);
    static DxSubscription* create(JNIEnv* env, dxfg_event_clazz_t eventType);
    static DxSubscription* create(JNIEnv* env, dxfg_event_clazz_list_t* eventClasses);
    static DxSubscription* create(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType);
    static DxSubscription* create(JNIEnv* env, jobject connection, dxfg_event_clazz_list_t* eventClasses);
    static DxSubscription* createTimeSeries(JNIEnv* env, jobject conn, dxfg_event_clazz_t eventType);
    static DxSubscription* createTimeSeries(JNIEnv* env, jobject conn, dxfg_event_clazz_list_t* eventClasses);
    ~DxSubscription();

    DxSubscription(const DxSubscription& other) = delete;
    DxSubscription(DxSubscription&& other) = delete;
    DxSubscription& operator=(const DxSubscription& other) = delete;
    DxSubscription& operator=(DxSubscription&& other) = delete;

    void close(JNIEnv* env);
    void addListener(JNIEnv* env, DxEventListener* listener);
    void removeListener(JNIEnv* env, DxEventListener* listener);
    int32_t addSymbol(JNIEnv* env, dxfg_symbol_t* symbol);
    int32_t addSymbols(JNIEnv* env, dxfg_symbol_list* symbols);
    int32_t removeSymbol(JNIEnv* env, dxfg_symbol_t* pSymbol);
    int32_t removeSymbols(JNIEnv* env, dxfg_symbol_list* symbols);
    int32_t clear(JNIEnv* env);
    int32_t attach(JNIEnv* env, DxFeed* pFeed);
    int32_t detach(JNIEnv* env, DxFeed* pFeed);
    int32_t isClosed(JNIEnv* env);
    dxfg_event_clazz_list_t* getEventTypes(JNIEnv* env);
    int32_t containsEventType(JNIEnv* env, dxfg_event_clazz_t eventTypeClass);
    dxfg_symbol_list* getSymbols(JNIEnv* env);
    int32_t setSymbol(JNIEnv* env, dxfg_symbol_t* symbol);
    int32_t setSymbols(JNIEnv* env, dxfg_symbol_list* symbols);
    dxfg_symbol_list* getDecoratedSymbols(JNIEnv* env);

    // DxFeedTimeSeriesSubscription methods
    int32_t setTime(JNIEnv* pEnv, int64_t time);

  private:
    jobject subscription_;
    std::thread consumer_thread;

    DxSubscription(JNIEnv* env, jobject dxSubscription);

    static jmethodID getMethodId(JNIEnv* env, jclass clazz, bool isTimeSeries, bool argIsArray);
    static jobjectArray buildJavaObjectArray(JNIEnv* env, const dxfg_event_clazz_list_t* eventClasses);
    static DxSubscription* create(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType, bool isTimeSeries);
    static DxSubscription* create(JNIEnv* env, jobject conn, dxfg_event_clazz_list_t* eventClasses, bool isTimeSeries);

    constexpr static const char DX_FEED_SUBSCRIPTION_CLASS_NAME[] = "com/dxfeed/api/DXFeedSubscription";
    constexpr static const char DX_FEED_SUBSCRIPTION_JNI_CLASS_NAME[] = "com/dxfeed/api/DxSubscriptionJni";
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_