// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_SDK_DXENDPOINT_H_
#define DXFEED_JNI_SDK_DXENDPOINT_H_

#include <jni.h>

namespace dxfeed {
  typedef void(* OnCloseHandler)(jobject clazz);


  struct DxEndpoint final {
    DxEndpoint(JNIEnv* env, dxfeed::OnCloseHandler onClose);
    ~DxEndpoint();

    jobject connect(const char* address);
    jobject getFeed(jobject DxEndpointConnected);
  private:
    jobject createDxEndpointBuilder();
    jobject createDxEndpoint(jobject dxEndpointBuilder);

    JNIEnv* env_;
    jclass dxEndpointClass_;
    jobject dxEndpoint_;
    const dxfeed::OnCloseHandler onClose_;
  };
}

#endif //DXFEED_JNI_SDK_DXENDPOINT_H_
