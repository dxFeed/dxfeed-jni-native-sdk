// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_DXPUBLISHER_H_
#define DXFEED_JNI_NATIVE_SDK_DXPUBLISHER_H_

#include <jni.h>

#include "api/dxfg_publisher.h"
#include "DxSubscription.hpp"

namespace dxfeed {
  struct DxPublisher {
    DxPublisher(JNIEnv* env, jobject obj);
    ~DxPublisher();

    static dxfg_publisher_t* getInstance(JNIEnv* env);
    int32_t publishEvents(JNIEnv* env, dxfg_event_type_list* events);
    DxObservableSubscription* getSubscription(JNIEnv* env, dxfg_event_clazz_t eventClazz);

    DxPublisher(const DxPublisher& other) = delete;
    DxPublisher(DxPublisher&& other) = delete;
    DxPublisher& operator=(const DxPublisher& other) = delete;
    DxPublisher& operator=(DxPublisher&& other) = delete;
  private:
    jobject dxPublisher_ = nullptr;

    constexpr static const char DX_PUBLISHER_CLASS_NAME[] = "com/dxfeed/api/DXPublisher";
//    constexpr static const char DX_PUBLISHER_JNI_CLASS_NAME[] = "com/dxfeed/api/DxFeedJni";
  };
}

#endif //DXFEED_JNI_NATIVE_SDK_DXPUBLISHER_H_
