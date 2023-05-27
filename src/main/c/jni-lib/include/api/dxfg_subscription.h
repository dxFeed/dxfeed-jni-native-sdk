// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_API_SUBSCRIPTION_H_
#define DXFEED_JNI_NATIVE_API_SUBSCRIPTION_H_

#ifdef __cplusplus
extern "C" {
#    include <cstdint>
#else
#    include <stdint.h>
#endif

#include "graal_isolate.h"
#include "dxfg_events.h"
#include "dxfg_javac.h"

/** @defgroup Subscription
 *  @{
 */

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html">Javadoc</a>
 */
typedef struct dxfg_subscription_t {
  dxfg_java_object_handler handler;
} dxfg_subscription_t;

typedef struct dxfg_feed_event_listener_t {
  dxfg_java_object_handler handler;
} dxfg_feed_event_listener_t;

typedef void (* dxfg_feed_event_listener_function)(graal_isolatethread_t* thread, dxfg_event_type_list* events,
                                                   void* user_data);

dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t* thread,
                                                         dxfg_feed_event_listener_function user_func, void* user_data);
int32_t                     dxfg_DXFeedEventListener_release(graal_isolatethread_t*,
                                                             dxfg_feed_event_listener_t* listener);

dxfg_subscription_t*  dxfg_DXFeedSubscription_new(graal_isolatethread_t* thread, dxfg_event_clazz_t eventClazz);
int32_t               dxfg_DXSubscription_release(graal_isolatethread_t*, dxfg_subscription_t* subscription);
int32_t               dxfg_DXFeedSubscription_close(graal_isolatethread_t* thread, dxfg_subscription_t* sub);
int32_t               dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                                               dxfg_feed_event_listener_t* listener);
int32_t               dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                                        dxfg_symbol_t* symbol);
int32_t               dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                                        dxfg_symbol_t* symbol);

/** @} */ // end of Subscription

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_SUBSCRIPTION_H_
