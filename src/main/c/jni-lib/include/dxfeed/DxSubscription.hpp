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
    constexpr static const char JAVA_CLASS_SUBSCRIPTION_NAME[] = "com.dxfeed.api.DXFeedSubscription";
    constexpr static const char JAVA_CLASS_TIME_SERIES_SUBSCRIPTION_NAME[] = "com.dxfeed.api.DXFeedSubscription";

    static DxSubscription* createSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType);
    static DxSubscription* createSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_list_t* eventClasses);
    static DxTimeSeriesSubscription* createTimeSeriesSubscription(JNIEnv* env, jobject connection,
                                                                  dxfg_event_clazz_t eventType);
    static DxTimeSeriesSubscription* createTimeSeriesSubscription(JNIEnv* env, jobject connection,
                                                                  dxfg_event_clazz_list_t* eventClasses);
    ~DxSubscription();

    DxSubscription(const DxSubscription& other) = delete;
    DxSubscription(DxSubscription&& other) = delete;
    DxSubscription& operator=(const DxSubscription& other) = delete;
    DxSubscription& operator=(DxSubscription&& other) = delete;

    void addListener(JNIEnv* env, DxEventListener* listener) const;
    void removeListener(JNIEnv* env, DxEventListener* listener) const;
    int32_t addSymbol(JNIEnv* env, dxfg_symbol_t* symbol) const;
    int32_t addSymbols(JNIEnv* env, dxfg_symbol_list* symbols) const;
    int32_t setSymbol(JNIEnv* env, dxfg_symbol_t* symbol) const;
    void close(JNIEnv* env) const;

    // DxFeedTimeSeriesSubscription methods
    int32_t setTime(JNIEnv* pEnv, int64_t time) const;

  private:
    jobject subscription_;

    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_t eventType, bool isTimeSeries);
    DxSubscription(JNIEnv* env, jobject connection, dxfg_event_clazz_list_t* eventClasses, bool isTimeSeries);
    static jmethodID getMethodId(JNIEnv* env, jclass clazz, bool isTimeSeries, bool argIsArray);
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_SUBSCRIPTION_H_