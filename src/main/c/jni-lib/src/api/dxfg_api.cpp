// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "api/dxfg_api.h"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNICommon.hpp"

dxfg_endpoint_builder_t* dxfg_DXEndpoint_newBuilder(graal_isolatethread_t* thread) {
  return reinterpret_cast <dxfg_endpoint_builder_t*>(new dxfeed::DxEndpointBuilder(thread));
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

dxfg_endpoint_t* dxfg_DXEndpoint_Builder_build(graal_isolatethread_t* thread, dxfg_endpoint_builder_t* builder) {
  auto pDxEndpointBuilder = reinterpret_cast<dxfeed::DxEndpointBuilder*>(builder);
  return reinterpret_cast <dxfg_endpoint_t*>(pDxEndpointBuilder->build(thread));
}

// todo: move to another CPP
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

dxfg_endpoint_state_change_listener_t* dxfg_PropertyChangeListener_new(graal_isolatethread_t* thread,
                                                                       dxfg_endpoint_state_change_listener_func userFunc,
                                                                       void* userData)
{
  auto stateChangeListener = new dxfeed::DxStateChangeListener(thread, userFunc, userData);
  return reinterpret_cast<dxfg_endpoint_state_change_listener_t*>(stateChangeListener);
}

int32_t dxfg_PropertyChangeListener_release(graal_isolatethread_t*, dxfg_endpoint_state_change_listener_t* listener) {
  auto* pListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(listener);
  delete pListener;
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_addStateChangeListener(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint,
                                               dxfg_endpoint_state_change_listener_t* listener)
{
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  auto* stateChangeListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(listener);
  pDxEndpoint->addStateChangeListener(thread, stateChangeListener);
  return JNI_OK;
}

int32_t dxfg_DXEndpoint_removeStateChangeListener(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint,
                                                  dxfg_endpoint_state_change_listener_t *listener)
{
  auto* pDxEndpoint = reinterpret_cast<dxfeed::DxEndpoint*>(endpoint);
  auto* stateChangeListener = reinterpret_cast<dxfeed::DxStateChangeListener*>(listener);
  pDxEndpoint->removeStateChangeListener(thread, stateChangeListener);
  return JNI_OK;
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription(graal_isolatethread_t* thread,
                                                    dxfg_feed_t* feed, dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(thread, eventClazz));
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription2(graal_isolatethread_t* thread,
                                                     dxfg_feed_t* feed, dxfg_event_clazz_list_t* eventClazzes)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(thread, eventClazzes));
}


int32_t dxfg_DXSubscription_release(graal_isolatethread_t*, dxfg_subscription_t* subscription) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(subscription);
  delete pDxSubscription;
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_close(graal_isolatethread_t* thread, dxfg_subscription_t* sub) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  pDxSubscription->close(thread);
  return JNI_OK;
}

dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t* thread,
                                                         dxfg_feed_event_listener_function user_func, void* user_data)
{
  return reinterpret_cast<dxfg_feed_event_listener_t*>(new dxfeed::DxEventListener(thread, user_func, user_data));
}

int32_t dxfg_DXFeedEventListener_release(graal_isolatethread_t*, dxfg_feed_event_listener_t* listener) {
  auto* pListener = reinterpret_cast<dxfeed::DxEventListener*>(listener);
  delete pListener;
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t* thread,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxEventListener =  reinterpret_cast<dxfeed::DxEventListener*>(listener);
  pDxSubscription->addListener(thread, pDxEventListener);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* symbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (symbol->type) {
    case STRING: {
      auto* pSymbol = reinterpret_cast<dxfg_string_symbol_t*>(symbol);
      // todo: investigate, why there no setSymbols method. Inlined?
      //  for now use addSymbol instead of setSymbol
      pDxSubscription->addSymbol(thread, pSymbol->symbol);
      break;
    }
    default:
      return JNI_ERR;
  }
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* symbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (symbol->type) {
    case STRING: {
      auto* pSymbol = reinterpret_cast<dxfg_string_symbol_t*>(symbol);
      pDxSubscription->addSymbol(thread, pSymbol->symbol);
      break;
    }
    default:
      return JNI_ERR;
  }
  return JNI_OK;
}

int dxfg_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler* object) {
  std::cout << "dxfg_JavaObjectHandler_release" << std::endl;
  if (object) {
    std::cout << "\tdeleteGlobalRef for object: " << std::hex << object << std::endl;
    jobject pObject = object->dxfg_java_object_handle;
    std::cout << "\tdxfg_java_object_handle: " << std::hex << pObject << std::endl;
    const auto& name = dxfeed::jni::internal::javaLangClass->getName(thread, pObject);
    if (!name.empty()) {
      std::cout << "\t" << name << std::endl;
      if (name.find(dxfeed::DxEndpoint::JAVA_CLASS_NAME) != std::string::npos) {
        dxfg_DXEndpoint_release(thread, (dxfg_endpoint_t*) object);
      } else if (name.find(dxfeed::DxSubscription::JAVA_CLASS_NAME) != std::string::npos) {
        dxfg_DXSubscription_release(thread, (dxfg_subscription_t*) object);
      } else if (name.find(dxfeed::DxStateChangeListener::JAVA_CLASS_NAME) != std::string::npos) {
        // todo: sync with Konstantin about DxStateChangeListener lifetime
//        dxfg_PropertyChangeListener_release(thread, (dxfg_endpoint_state_change_listener_t*) object);
      } else if (name.find(dxfeed::DxEventListener::JAVA_CLASS_NAME) != std::string::npos) {
        dxfg_DXFeedEventListener_release(thread, (dxfg_feed_event_listener_t*) object);
      } else {
        std::cerr << "\t LEAKED: " << std::hex << pObject << std::endl;
      }
      return JNI_OK;
    }
  }
  return JNI_OK; // todo: think
}



