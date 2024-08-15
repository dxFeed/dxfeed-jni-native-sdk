// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_DXFEEDJNICLASS_H
#define NATIVE_JNI_SDK_DXFEEDJNICLASS_H

#include "javah/com_dxfeed_api_DxEndpointJni.h"
#include "javah/com_dxfeed_api_DxSubscriptionJni.h"
#include <jni.h>

namespace dxfeed::jni {
void initDxJni(JNIEnv *env);

constexpr JNINativeMethod build(const char *name, const char *signature, void *fnPtr) {
    return JNINativeMethod{const_cast<char *>(name), const_cast<char *>(signature), fnPtr};
}

static JNINativeMethod nDxEndpoint =
    build("nOnStateChangeListener", "(IIJJ)V", (void *)&Java_com_dxfeed_api_DxEndpointJni_nOnStateChangeListener);

static JNINativeMethod nDxSubscription =
    build("nOnEventListener", "(I[B[D[BJJ)V", (void *)&Java_com_dxfeed_api_DxSubscriptionJni_nOnEventListener);
} // namespace dxfeed::jni

#endif // NATIVE_JNI_SDK_DXFEEDJNICLASS_H