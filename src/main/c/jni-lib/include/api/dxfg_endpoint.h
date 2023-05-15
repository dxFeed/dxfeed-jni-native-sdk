// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_API_ENDPOINT_H_
#define DXFEED_JNI_NATIVE_API_ENDPOINT_H_

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

/**
 * @brief The DXEndpoint.
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.html">Javadoc</a>
 */
typedef struct dxfg_endpoint_t {
  dxfg_java_object_handler handler;
} dxfg_endpoint_t;

/**
 * @brief The DXEndpoint.Builder.
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Builder.html">Javadoc</a>
 */
typedef struct dxfg_endpoint_builder_t {
  dxfg_java_object_handler handler;
} dxfg_endpoint_builder_t;

/**
 * @brief List of endpoint roles.
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXEndpoint.Role.html">Javadoc</a>
 */
typedef enum dxfg_endpoint_role_t {
  DXFG_ENDPOINT_ROLE_FEED = 0,
  DXFG_ENDPOINT_ROLE_ON_DEMAND_FEED,
  DXFG_ENDPOINT_ROLE_STREAM_FEED,
  DXFG_ENDPOINT_ROLE_PUBLISHER,
  DXFG_ENDPOINT_ROLE_STREAM_PUBLISHER,
  DXFG_ENDPOINT_ROLE_LOCAL_HUB,
} dxfg_endpoint_role_t;
/** @defgroup Builder
 *  @{
 */
dxfg_endpoint_builder_t* dxfg_DXEndpoint_newBuilder(graal_isolatethread_t* thread);
int32_t dxfg_DXEndpoint_Builder_withRole(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                         dxfg_endpoint_role_t role);
int32_t dxfg_DXEndpoint_Builder_withName(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                         const char* name);
int32_t dxfg_DXEndpoint_Builder_withProperty(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                             const char* key, const char* value);
int32_t dxfg_DXEndpoint_Builder_withProperties(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                               const char* filePath);
int32_t dxfg_DXEndpoint_Builder_supportsProperty(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                                 const char* key);
dxfg_endpoint_t*         dxfg_DXEndpoint_Builder_build(graal_isolatethread_t *thread, dxfg_endpoint_builder_t *builder);

/** @} */ // end of Builder

dxfg_endpoint_t*                dxfg_DXEndpoint_getInstance(graal_isolatethread_t* thread);
dxfg_endpoint_t*                dxfg_DXEndpoint_create(graal_isolatethread_t* thread);
int32_t                         dxfg_DXEndpoint_release(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint);
int32_t                         dxfg_DXEndpoint_close(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint);
int32_t                         dxfg_DXEndpoint_connect(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint,
                                                        const char* address);
dxfg_feed_t*                    dxfg_DXEndpoint_getFeed(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint);

int32_t                         dxfg_DXEndpoint_awaitNotConnected(graal_isolatethread_t* thread,
                                                                  dxfg_endpoint_t* endpoint);

/** @} */ // end of Endpoint

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_ENDPOINT_H_