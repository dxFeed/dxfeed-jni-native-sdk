// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_API_FEED_H_
#define DXFEED_JNI_NATIVE_API_FEED_H_

#ifdef __cplusplus
extern "C" {
#    include <cstdint>
#else
#    include <stdint.h>
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

/**
 * @brief The DXFeed.
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeed.html">Javadoc</a>
 */
typedef struct dxfg_feed_t {
    dxfg_java_object_handler handler;
} dxfg_feed_t;


dxfg_feed_t*                      dxfg_DXFeed_getInstance(graal_isolatethread_t *thread);
dxfg_subscription_t*              dxfg_DXFeed_createSubscription(graal_isolatethread_t *thread, dxfg_feed_t *feed, dxfg_event_clazz_t eventClazz);
dxfg_subscription_t*              dxfg_DXFeed_createSubscription2(graal_isolatethread_t *thread, dxfg_feed_t *feed, dxfg_event_clazz_list_t *eventClazzes);

/** @} */ // end of Feed

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_FEED_H_