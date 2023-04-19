// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_SDK_DXENDPOINT_H_
#define DXFEED_JNI_SDK_DXENDPOINT_H_

#include <jni.h>
#include <cstdint>

namespace dxfeed {
  typedef void(* OnCloseHandler)(jobject clazz);


  struct DxEndpoint final {
    DxEndpoint(JNIEnv* env, dxfeed::OnCloseHandler onClose);
    ~DxEndpoint();

    int32_t connect(const char* address);
    jobject getFeed(jobject DxEndpointConnected);
  private:
    jobject createDxEndpointBuilder();
    jobject createDxEndpoint(jobject dxEndpointBuilder);

    jobject dxEndpoint_;
    jclass dxEndpointClass_;
    const dxfeed::OnCloseHandler onClose_;
    JNIEnv* env_;
  };
}

#endif //DXFEED_JNI_SDK_DXENDPOINT_H_
