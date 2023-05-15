// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxStateChangeListener.hpp"

namespace dxfeed {
  DxStateChangeListener::DxStateChangeListener(dxfg_endpoint_state_change_listener_func userFunc, void* userData) {
    userFunc_ = userFunc;
    userData_ = userData;
  }

  void DxStateChangeListener::callUserFunc(graal_isolatethread_t* thread, int32_t oldStateEnum, int32_t newStateEnum) {
    auto oldState = static_cast<dxfg_endpoint_state_t>(oldStateEnum);
    auto newState = static_cast<dxfg_endpoint_state_t>(newStateEnum);
    userFunc_(thread, oldState, newState, userData_);
  }
}