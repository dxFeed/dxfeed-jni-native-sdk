#pragma once

#include <jni.h>
#include <string>
#include <vector>
#include <memory>

#include "Subscription.h"
#include "EventTypes.h"

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
}
