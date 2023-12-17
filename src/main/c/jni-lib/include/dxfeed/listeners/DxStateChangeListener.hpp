// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
#define DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_

#include <jni.h>

#include "api/dxfg_endpoint.h"

namespace dxfeed {
  struct DxStateChangeListener {
    static DxStateChangeListener* create(JNIEnv* env, dxfg_endpoint_state_change_listener_func callback,
                                         void* userData);
  private:
    explicit DxStateChangeListener(jlong listenerId);
    jobject dummy_ = nullptr;
  public:
    const jlong javaListenerId_;
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXSTATECHANGELISTENER_H_
