// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NATIVE_JNI_SDK_DXFEEDJNICLASS_H
#define NATIVE_JNI_SDK_DXFEEDJNICLASS_H

#include <jni.h>
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include "javah/com_dxfeed_api_DxEndpointJni.h"

namespace dxfeed::jni {
  void initDxJni(JNIEnv* env);

  constexpr JNINativeMethod build(const char* name, const char* signature, void* fnPtr) {
    return JNINativeMethod {const_cast<char*>(name), const_cast<char*>(signature), fnPtr};
  }

  static JNINativeMethod nDxEndpoint = build("nOnStateChangeListener", "(IIJJ)V",
                                      (void*) &Java_com_dxfeed_api_DxEndpointJni_nOnStateChangeListener);

  static JNINativeMethod nDxSubscription = build( "nOnEventListener", "(I[B[D[BJJ)V",
                                           (void*) &Java_com_dxfeed_api_DxSubscriptionJni_nOnEventListener);
}

#endif //NATIVE_JNI_SDK_DXFEEDJNICLASS_H