// SPDX-License-Identifier: MPL-2.0

#include <sstream>

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_endpoint_state_change_listener_t* dxfg_PropertyChangeListener_new(graal_isolatethread_t* thread,
                                                                       dxfg_endpoint_state_change_listener_func userFunc,
                                                                       void* userData)
{
  auto* pDxStateChangeListener = dxfeed::DxStateChangeListener::create(thread, userFunc, userData);
  return dxfeed::r_cast<dxfg_endpoint_state_change_listener_t*>(pDxStateChangeListener);
}

dxfg_feed_event_listener_t* dxfg_DXFeedEventListener_new(graal_isolatethread_t* thread,
                                                         dxfg_feed_event_listener_function user_func, void* user_data)
{
  auto* pDxEventListener = dxfeed::DxEventListener::create(thread, user_func, user_data);
  return dxfeed::r_cast<dxfg_feed_event_listener_t*>(pDxEventListener);
}

int dxfg_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler* object) {
  if (object) {
    std::stringstream ss;
    ss << "dxfg_JavaObjectHandler_release: ";
    auto pObject = object->dxfg_java_object_handle;
    ss << "\t dxfg_java_object_handle: " << pObject;
    const auto& name = pObject ? dxfeed::jni::internal::javaLangClass->getName(thread, pObject) : "";
    if (!name.empty()) {
      ss << ", name: " << name;
      if (name == dxfeed::DxEndpoint::JAVA_CLASS_NAME) {
        dxfg_DXEndpoint_release(thread, dxfeed::r_cast<dxfg_endpoint_t*>(object));
      } else if (name == dxfeed::DxEndpointBuilder::JAVA_CLASS_NAME) {
        dxfg_DXEndpointBuilder_release(thread, dxfeed::r_cast<dxfg_endpoint_builder_t*>(object));
      } else if (
        name == dxfeed::DxSubscription::JAVA_SUBSCRIPTION_CLASS_NAME ||
        name == dxfeed::DxSubscription::JAVA_TIME_SERIES_SUBSCRIPTION_NAME
      ) {
        dxfg_DXSubscription_release(thread, dxfeed::r_cast<dxfg_subscription_t*> (object));
      } else {
        ss << ", LEAKED: " << std::hex << pObject;
      }
    } else {
      ss << ", already released: " << std::hex << pObject;
      delete object;
    }
    dxfeed::jni::javaLogger->trace(thread, ss.str());
  }
  return JNI_OK;
}



