// SPDX-License-Identifier: MPL-2.0

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
    javaLogger->info("com.dxfeed.api.DxFeedJni$NativeEventData: %", dxNativeEventDataClass_);
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
    
    auto byteData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(byteArray, 0));
    auto doubleData = r_cast<const double*>(env_->GetPrimitiveArrayCritical(doubleArray, 0));

    // return newEventType via arg-pointer
    dxfg_event_type_t* pEventType = *ppEventType;
    dxfg_event_type_t* pType = NativeEventReader::toEvent(&byteData, &doubleData, pEventType->clazz);
    delete pEventType;
    *ppEventType = pType;

    env_->ReleasePrimitiveArrayCritical(byteArray, const_cast<char*>(byteData), 0);
    env_->ReleasePrimitiveArrayCritical(doubleArray, const_cast<double*>(doubleData), 0);
    env_->DeleteLocalRef(doubleArray);
    env_->DeleteLocalRef(byteArray);
  }
}
