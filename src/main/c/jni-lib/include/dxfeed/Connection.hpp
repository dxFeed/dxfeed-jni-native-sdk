// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_CONNECTION_H_
#define DXFEED_JNI_NATIVE_SDK_CONNECTION_H_

#include <jni.h>
#include <string>
#include <vector>
#include <memory>

#include "Subscription.hpp"
#include "api/EventTypes.h"

namespace dxfeed {
  struct Connection final {
    Connection(JNIEnv* env, const std::string& address, dxfeed::OnCloseHandler);
    Connection(const Connection& other) = delete;
    Connection(Connection&& other) = delete;
    Connection& operator=(const Connection& other) = delete;
    Connection& operator=(Connection&& other) = delete;
    Subscription* createSubscription(EventType eventType);

    ~Connection();
  private:
    JNIEnv* env_;
    jobject endpoint_;
    jobject connection_;
    const dxfeed::OnCloseHandler onClose_;
  };
} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_CONNECTION_H_