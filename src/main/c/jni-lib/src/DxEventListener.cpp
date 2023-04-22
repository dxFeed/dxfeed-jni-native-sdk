// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxEventListener.hpp"

namespace dxfeed {
  DxEventListener::DxEventListener(dxfg_feed_event_listener_function userFunc, void* userData) {
    userFunc_ = userFunc;
    userData_ = userData;
  }

  void DxEventListener::callUserFunc(graal_isolatethread_t* thread, dxfg_event_type_list* events) {
    userFunc_(thread, events, userData_);
  }
}