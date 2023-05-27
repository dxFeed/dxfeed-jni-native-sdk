// SPDX-License-Identifier: MPL-2.0

#include <iostream>

#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNICommon.hpp"
#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/DxSubscription.hpp"

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

int dxfg_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler* object) {
  std::cout << "dxfg_JavaObjectHandler_release" << std::endl;
  if (object) {
    std::cout << "\tdeleteGlobalRef for object: " << std::hex << object << std::endl;
    jobject pObject = object->dxfg_java_object_handle;
    std::cout << "\tdxfg_java_object_handle: " << std::hex << pObject << std::endl;
    const auto& name = dxfeed::jni::internal::javaLangClass->getName(thread, pObject);
    if (!name.empty()) {
      std::cout << "\t" << name << std::endl;
      if (name == dxfeed::DxEndpoint::JAVA_CLASS_NAME) {
        dxfg_DXEndpoint_release(thread, reinterpret_cast<dxfg_endpoint_t*>(object));
      } else if (name == dxfeed::DxEndpointBuilder::JAVA_CLASS_NAME) {
        dxfg_DXEndpointBuilder_release(thread, reinterpret_cast<dxfg_endpoint_builder_t*>(object));
      } else if (name == dxfeed::DxSubscription::JAVA_CLASS_NAME) {
        dxfg_DXSubscription_release(thread, reinterpret_cast<dxfg_subscription_t*> (object));
      } else if (name == dxfeed::DxStateChangeListener::JAVA_CLASS_NAME) {
        // todo: sync with Konstantin about DxStateChangeListener lifetime
//        dxfg_PropertyChangeListener_release(thread, (dxfg_endpoint_state_change_listener_t*) object);
        std::cerr << "\t LEAKED: " << std::hex << pObject << std::endl;
      } else if (name == dxfeed::DxEventListener::JAVA_CLASS_NAME) {
        dxfg_DXFeedEventListener_release(thread, (dxfg_feed_event_listener_t*) object);
      } else {
        std::cerr << "\t LEAKED: " << std::hex << pObject << std::endl;
      }
    } else {
      std::cerr << "\t LEAKED: " << std::hex << pObject << std::endl;
    }
    thread->DeleteLocalRef(pObject);
  }
  return JNI_OK; // todo: think
}



