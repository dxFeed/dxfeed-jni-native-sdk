// SPDX-License-Identifier: MPL-2.0

#include <memory>

#include "api/dxfg_endpoint.h"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"

/** @defgroup Builder
 *  @{
 */

dxfg_endpoint_builder_t* dxfg_DXEndpoint_newBuilder(graal_isolatethread_t* thread) {
  return reinterpret_cast <dxfg_endpoint_builder_t*>(new dxfeed::DxEndpointBuilder(thread));
}

int32_t dxfg_DXEndpointBuilder_release(graal_isolatethread_t*, dxfg_endpoint_builder_t* endpointBuilder) {
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(endpointBuilder);
  delete pDxEndpointBuilder;
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_Builder_withRole(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                         dxfg_endpoint_role_t role)
{
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  pDxEndpointBuilder->withRole(thread, role);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_Builder_withName(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                         const char* name)
{
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  pDxEndpointBuilder->withName(thread, name);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_Builder_withProperty(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                             const char* key, const char* value)
{
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  pDxEndpointBuilder->withProperty(thread, key, value);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_Builder_withProperties(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                               const char* filePath)
{
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  pDxEndpointBuilder->withProperties(thread, filePath);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_Builder_supportsProperty(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder,
                                                 const char* key)
{
  return JNI_OK;
}

dxfg_endpoint_t* dxfg_DXEndpoint_Builder_build(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder)
{
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  return reinterpret_cast <dxfg_endpoint_t*>(pDxEndpointBuilder->build(thread));
}

/** @} */ // end of Builder

dxfg_endpoint_t* dxfg_DXEndpoint_getInstance(graal_isolatethread_t* thread) {
  return reinterpret_cast<dxfg_endpoint_t*>(dxfeed::DxEndpoint::getInstance(thread));
}

dxfg_endpoint_t* dxfg_DXEndpoint_create(graal_isolatethread_t* thread) {
  auto pBuilder = std::make_unique<dxfeed::DxEndpointBuilder>(thread);
  dxfeed::DxEndpoint* pEndpoint = pBuilder->build(thread);
  return reinterpret_cast<dxfg_endpoint_t*>(pEndpoint);
}

int32_t dxfg_DXEndpoint_release(graal_isolatethread_t*, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  delete pDxEndpoint;
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_close(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  pDxEndpoint->close(thread);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_connect(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint, const char* address) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  return pDxEndpoint->connect(thread, address);
}

dxfg_feed_t* dxfg_DXEndpoint_getFeed(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  return reinterpret_cast<dxfg_feed_t*>(pDxEndpoint->getFeed(thread));
}

int32_t dxfg_DXEndpoint_awaitNotConnected(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint) {
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  pDxEndpoint->awaitNotConnected(thread);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_addStateChangeListener(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint,
                                               dxfg_endpoint_state_change_listener_t* listener)
{
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  auto* stateChangeListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(listener);
  pDxEndpoint->addStateChangeListener(thread, stateChangeListener);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_removeStateChangeListener(graal_isolatethread_t* thread, dxfg_endpoint_t* endpoint,
                                                  dxfg_endpoint_state_change_listener_t* listener)
{
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  auto* stateChangeListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(listener);
  pDxEndpoint->removeStateChangeListener(thread, stateChangeListener);
  return JNI_OK;
}