// SPDX-License-Identifier: MPL-2.0

#include <iostream>
#include "dxfeed/utils/NativeEventData.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/NativeEventReader.hpp"

namespace dxfeed::jni {
  NativeEventData::NativeEventData(JNIEnv* env):
    env_(env),
    dxNativeEventDataClass_(
      r_cast<jclass>(safeFindClass(env, "Lcom/dxfeed/api/DxFeedJni$NativeEventData;"))
    )
  {
    std::cout << "com.dxfeed.api.DxFeedJni$NativeEventData: " << dxNativeEventDataClass_ << "\n";
    byteData_ = safeGetFieldID(env, dxNativeEventDataClass_, "byteData", "[B");
    doubleData_ = safeGetFieldID(env, dxNativeEventDataClass_, "doubleData", "[D");
  }

  NativeEventData::~NativeEventData() {
    env_->DeleteLocalRef(dxNativeEventDataClass_);
    byteData_ = nullptr;
    doubleData_ = nullptr;
    env_ = nullptr;
  }

  void NativeEventData::toNativeEvent(jobject nativeEventData, dxfg_event_type_t** ppEventType) {
    auto byteArray = r_cast<jbyteArray>(env_->GetObjectField(nativeEventData, byteData_));
    auto doubleArray = r_cast<jdoubleArray>(env_->GetObjectField(nativeEventData, doubleData_));
    
    auto byteData = r_cast<char*>(env_->GetPrimitiveArrayCritical(byteArray, 0));
    auto doubleData = r_cast<double*>(env_->GetPrimitiveArrayCritical(doubleArray, 0));

    // return newEventType via arg-pointer
    dxfg_event_type_t* pEventType = *ppEventType;
    dxfg_event_type_t* pType = jni::NativeEventReader::toEvent(byteData, doubleData, pEventType->clazz);
    delete pEventType;
    *ppEventType = pType;

    env_->ReleasePrimitiveArrayCritical(byteArray, r_cast<void*>(byteData), 0);
    env_->ReleasePrimitiveArrayCritical(doubleArray, r_cast<void*>(doubleData), 0);
    env_->DeleteLocalRef(doubleArray);
    env_->DeleteLocalRef(byteArray);
  }
}
