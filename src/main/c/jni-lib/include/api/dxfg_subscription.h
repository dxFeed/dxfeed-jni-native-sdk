// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_API_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_API_SUBSCRIPTION_H_

#ifdef __cplusplus
extern "C" {
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "graal_isolate.h"
#include "dxfg_events.h"
#include "dxfg_javac.h"

/** @defgroup Subscription
 *  @{
 */

/**
 * @brief Forward declarations.
 */
typedef struct dxfg_feed_t dxfg_feed_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html">Javadoc</a>
 */
typedef struct dxfg_subscription_t {
  dxfg_java_object_handler handler;
} dxfg_subscription_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedTimeSeriesSubscription.html">Javadoc</a>
 */
typedef struct dxfg_time_series_subscription_t {
  dxfg_subscription_t sub;
} dxfg_time_series_subscription_t;

typedef struct dxfg_feed_event_listener_t {
  dxfg_java_object_handler handler;
} dxfg_feed_event_listener_t;

typedef void (* dxfg_feed_event_listener_function)(graal_isolatethread_t* thread, dxfg_event_type_list* events, void* user_data);
dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t* thread, dxfg_feed_event_listener_function user_func, void* user_data);

dxfg_subscription_t*      dxfg_DXFeedSubscription_new(graal_isolatethread_t* thread, dxfg_event_clazz_t eventClass);
dxfg_subscription_t*      dxfg_DXFeedSubscription_new2(graal_isolatethread_t* thread, dxfg_event_clazz_list_t* eventClasses);
int32_t                   dxfg_DXSubscription_release(graal_isolatethread_t*, dxfg_subscription_t* subscription);
int32_t                   dxfg_DXFeedSubscription_close(graal_isolatethread_t* thread, dxfg_subscription_t* sub);
int32_t                   dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener);
int32_t                   dxfg_DXFeedSubscription_removeEventListener(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener);
int32_t                   dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_symbol_t* pSymbol);
int32_t                   dxfg_DXFeedSubscription_addSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_symbol_list* symbols);
int32_t                   dxfg_DXFeedSubscription_removeSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_symbol_t* symbol);
int32_t                   dxfg_DXFeedSubscription_removeSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_symbol_list* symbols);
int32_t                   dxfg_DXFeedSubscription_clear(graal_isolatethread_t *thread, dxfg_subscription_t *sub);
int32_t                   dxfg_DXFeedSubscription_attach(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_feed_t* feed);
int32_t                   dxfg_DXFeedSubscription_detach(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_feed_t* feed);
int32_t                   dxfg_DXFeedSubscription_isClosed(graal_isolatethread_t* thread, dxfg_subscription_t* sub);
dxfg_event_clazz_list_t*  dxfg_DXFeedSubscription_getEventTypes(graal_isolatethread_t* thread, dxfg_subscription_t* sub);
int32_t                   dxfg_DXFeedSubscription_containsEventType(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_event_clazz_t eventClazz);
dxfg_symbol_list*         dxfg_DXFeedSubscription_getSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub);
int32_t                   dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_symbol_t* symbol);
int32_t                   dxfg_DXFeedSubscription_setSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_symbol_list* symbols);
dxfg_symbol_list*         dxfg_DXFeedSubscription_getDecoratedSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub);
/* todo: implement later
dxfg_executor_t*          dxfg_DXFeedSubscription_getExecutor(graal_isolatethread_t *thread, dxfg_subscription_t *sub);
int32_t                   dxfg_DXFeedSubscription_setExecutor(graal_isolatethread_t *thread, dxfg_executor_t *executor);
int32_t                   dxfg_DXFeedSubscription_addChangeListener(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_observable_subscription_change_listener_t* listener);
int32_t                   dxfg_DXFeedSubscription_removeChangeListener(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_observable_subscription_change_listener_t* listener);
 */
int32_t                   dxfg_DXFeedTimeSeriesSubscription_setFromTime(graal_isolatethread_t* thread, dxfg_time_series_subscription_t* sub, int64_t fromTime);

/** @} */ // end of Subscription

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_SUBSCRIPTION_H_
