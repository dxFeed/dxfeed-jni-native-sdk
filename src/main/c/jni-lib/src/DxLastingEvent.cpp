// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxLastingEvent.hpp"
#include "dxfeed/utils/JNICommon.hpp"

namespace dxfeed {
  DxLastingEvent::DxLastingEvent(JNIEnv* env, jobject dxLastingEvent) :
      dxLastingEvent_(env->NewGlobalRef(dxLastingEvent)),
      dxLastingEventClass_(env->GetObjectClass(dxLastingEvent))
  {}

  DxLastingEvent::~DxLastingEvent() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(dxLastingEvent_);
  }
}
