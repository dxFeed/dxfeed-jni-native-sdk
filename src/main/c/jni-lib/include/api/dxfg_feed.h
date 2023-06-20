// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_API_FEED_H_
#define DXFEED_JNI_NATIVE_API_FEED_H_

#ifdef __cplusplus
extern "C" {
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "dxfg_events.h"
#include "dxfg_javac.h"
#include "graal_isolate.h"

/** @defgroup Feed
 *  @{
 */

/**
 * @brief Forward declarations.
 */
typedef struct dxfg_subscription_t dxfg_subscription_t;
typedef struct dxfg_time_series_subscription_t dxfg_time_series_subscription_t;

/**
 * @brief The DXFeed.
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html">Javadoc</a>
 */
typedef struct dxfg_feed_t {
  dxfg_java_object_handler handler;
} dxfg_feed_t;


dxfg_feed_t*                      dxfg_DXFeed_getInstance(graal_isolatethread_t* thread);
dxfg_subscription_t*              dxfg_DXFeed_createSubscription(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                                                 dxfg_event_clazz_t eventClazz);
dxfg_subscription_t*              dxfg_DXFeed_createSubscription2(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                                                  dxfg_event_clazz_list_t* eventClazzes);
dxfg_time_series_subscription_t*  dxfg_DXFeed_createTimeSeriesSubscription(graal_isolatethread_t* thread,
                                                                           dxfg_feed_t* feed,
                                                                           dxfg_event_clazz_t eventClazz);
dxfg_time_series_subscription_t*  dxfg_DXFeed_createTimeSeriesSubscription2(graal_isolatethread_t* thread,
                                                                            dxfg_feed_t* feed,
                                                                            dxfg_event_clazz_list_t* eventClazzes);
int32_t                           dxfg_DXFeed_attachSubscription(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                                                 dxfg_subscription_t* sub);
int32_t                           dxfg_DXFeed_detachSubscription(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                                                 dxfg_subscription_t* sub);
int32_t                           dxfg_DXFeed_detachSubscriptionAndClear(graal_isolatethread_t* thread,
                                                                         dxfg_feed_t* feed,
                                                                         dxfg_subscription_t* sub);
dxfg_event_type_t*                dxfg_DXFeed_getLastEventIfSubscribed(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                                                       dxfg_event_clazz_t eventClazz,
                                                                       dxfg_symbol_t* symbol);
dxfg_event_type_list*             dxfg_DXFeed_getIndexedEventsIfSubscribed(graal_isolatethread_t* thread,
                                                                           dxfg_feed_t* feed,
                                                                           dxfg_event_clazz_t eventClazz,
                                                                           dxfg_symbol_t* symbol, const char* source);
dxfg_event_type_list*             dxfg_DXFeed_getTimeSeriesIfSubscribed(graal_isolatethread_t* thread,
                                                                        dxfg_feed_t* feed,
                                                                        dxfg_event_clazz_t eventClazz,
                                                                        dxfg_symbol_t* symbol, int64_t from_time,
                                                                        int64_t to_time);

// use dxfg_EventType_new to create an empty structure so that java tries to free up memory when replacing subjects
int32_t dxfg_DXFeed_getLastEvent(graal_isolatethread_t* thread, dxfg_feed_t* feed, dxfg_event_type_t* event);
int32_t dxfg_DXFeed_getLastEvents(graal_isolatethread_t* thread, dxfg_feed_t* feed, dxfg_event_type_list* events);

int32_t                           dxfg_DXFeedTimeSeriesSubscription_setFromTime(graal_isolatethread_t* thread,
                                                                                dxfg_time_series_subscription_t* sub,
                                                                                int64_t fromTime);


/** @} */ // end of Feed

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_FEED_H_
