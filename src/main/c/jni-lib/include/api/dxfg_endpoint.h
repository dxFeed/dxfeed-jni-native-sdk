// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_GRAAL_NATIVE_API_ENDPOINT_H_
#define DXFEED_GRAAL_NATIVE_API_ENDPOINT_H_

#ifdef __cplusplus
extern "C" {
#    include <cstdint>
#else
#    include <stdint.h>
#endif

#include "graal_isolate.h"
#include "dxfg_javac.h"

/** @defgroup Endpoint
 *  @{
 */

/**
 * @brief Forward declarations.
 */
typedef struct dxfg_feed_t dxfg_feed_t;
typedef struct dxfg_publisher_t dxfg_publisher_t;
typedef struct dxfg_event_clazz_list_t dxfg_event_clazz_list_t;

/**
 * @brief The DXEndpoint.
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.html">Javadoc</a>
 */
typedef dxfg_java_object_handler dxfg_endpoint_t;

dxfg_endpoint_t*                dxfg_DXEndpoint_getInstance(graal_isolatethread_t *thread);
dxfg_endpoint_t*                dxfg_DXEndpoint_create(graal_isolatethread_t *thread);
int32_t                         dxfg_DXEndpoint_close(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint);
dxfg_endpoint_t*                dxfg_DXEndpoint_connect(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint, const char *address);
dxfg_feed_t*                    dxfg_DXEndpoint_getFeed(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint);
dxfg_event_clazz_list_t*        dxfg_DXEndpoint_getEventTypes(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint);

/** @} */ // end of Endpoint

#ifdef __cplusplus
}
#endif

#endif // DXFEED_GRAAL_NATIVE_API_ENDPOINT_H_
