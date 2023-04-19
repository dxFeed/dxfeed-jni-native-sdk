// SPDX-License-Identifier: MPL-2.0

#include "api/EventTypes.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/DXEndpoint.hpp"
#include "dxfeed/Connection.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {

  Connection::Connection(JNIEnv* env, const std::string& address, dxfeed::OnCloseHandler onClose) :
    env_{env},
    onClose_(onClose)
  {
//    connection_ = env->NewGlobalRef(createDxFeedConnect(env, endpoint_, address.c_str()));
  }

  Subscription* Connection::createSubscription(EventType eventType) {
    return new Subscription(env_, connection_, eventType, onClose_);
  }

  Connection::~Connection() {
    onClose_(endpoint_);
    onClose_(connection_);
  }
}