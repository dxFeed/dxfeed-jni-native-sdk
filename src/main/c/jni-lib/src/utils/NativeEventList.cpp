// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/NativeEventList.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/NativeEventReader.hpp"

namespace dxfeed::jni {
  NativeEventList::NativeEventList(JNIEnv* env):
    env_(env),
    dxNativeEventListClass_(r_cast<jclass>(safeFindClass(env, "Lcom/dxfeed/api/NativeEventList;"))),
    dxByteBuffer_(r_cast<jclass>(safeFindClass(env, "Lcom/dxfeed/api/buffers/ByteBuffer;"))),
    dxDoubleBuffer_(r_cast<jclass>(safeFindClass(env, "Lcom/dxfeed/api/buffers/DoubleBuffer;")))
  {
    javaLogger->info("com.dxfeed.api.NativeEventList: %", dxNativeEventListClass_);
    byteBuffer_ = safeGetFieldID(env, dxNativeEventListClass_, "pBytes", "Lcom/dxfeed/api/buffers/ByteBuffer;");
    doubleBuffer_ = safeGetFieldID(env, dxNativeEventListClass_, "pDoubles", "Lcom/dxfeed/api/buffers/DoubleBuffer;");
    pEventTypes_ = safeGetFieldID(env, dxNativeEventListClass_, "pEventTypes", "[B");

    toByteData_ = safeGetMethodID(env_, dxByteBuffer_, "toByteData", "()[B");
    toDoubleData_ = safeGetMethodID(env_, dxDoubleBuffer_, "toDoubleData", "()[B");
  }

  NativeEventList::~NativeEventList() {
    env_->DeleteLocalRef(dxDoubleBuffer_);
    env_->DeleteLocalRef(dxByteBuffer_);
    env_->DeleteLocalRef(dxNativeEventListClass_);

    toDoubleData_ = nullptr;
    toByteData_ = nullptr;
    doubleBuffer_ = nullptr;
    byteBuffer_ = nullptr;
    pEventTypes_ = nullptr;
    env_ = nullptr;
  }

  void NativeEventList::toNativeEvent(jobject nativeEventList, dxfg_event_type_t** ppEventType) {
    jobject byteBuffer = env_->GetObjectField(nativeEventList, byteBuffer_);
    jobject doubleBuffer = env_->GetObjectField(nativeEventList, doubleBuffer_);

    auto byteArray = r_cast<jbyteArray>(env_->CallObjectMethod(byteBuffer, toByteData_));
    auto doubleArray = r_cast<jbyteArray>(env_->CallObjectMethod(doubleBuffer, toDoubleData_));
    auto eventTypeArray = r_cast<jdoubleArray>(env_->GetObjectField(nativeEventList, pEventTypes_));
    jint size = env_->GetArrayLength(eventTypeArray);

    auto byteData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(byteArray, 0));
    auto doubleData = r_cast<const double*>(env_->GetPrimitiveArrayCritical(doubleArray, 0));
    auto eventTypesData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(eventTypeArray, 0));

    // return newEventType via arg-pointer
    const auto& vector = NativeEventReader::toEvents(size, byteData, doubleData, eventTypesData);
    dxfg_event_type_t* pType = vector[0];
    *ppEventType = pType;

    env_->ReleasePrimitiveArrayCritical(byteArray, const_cast<char*>(byteData), 0);
    env_->ReleasePrimitiveArrayCritical(doubleArray, const_cast<double*>(doubleData), 0);
    env_->ReleasePrimitiveArrayCritical(eventTypeArray, const_cast<char*>(eventTypesData), 0);
    env_->DeleteLocalRef(eventTypeArray);
    env_->DeleteLocalRef(doubleArray);
    env_->DeleteLocalRef(byteArray);
    env_->DeleteLocalRef(doubleBuffer);
    env_->DeleteLocalRef(byteBuffer);
  }

  void NativeEventList::toNativeEventList(jobject nativeEventList, dxfg_event_type_list** ppEventTypeList) {
    jobject byteBuffer = env_->GetObjectField(nativeEventList, byteBuffer_);
    jobject doubleBuffer = env_->GetObjectField(nativeEventList, doubleBuffer_);

    auto byteArray = r_cast<jbyteArray>(env_->CallObjectMethod(byteBuffer, toByteData_));
    auto doubleArray = r_cast<jbyteArray>(env_->CallObjectMethod(doubleBuffer, toDoubleData_));
    auto eventTypeArray = r_cast<jdoubleArray>(env_->GetObjectField(nativeEventList, pEventTypes_));
    jint size = env_->GetArrayLength(eventTypeArray);

    auto byteData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(byteArray, 0));
    auto doubleData = r_cast<const double*>(env_->GetPrimitiveArrayCritical(doubleArray, 0));
    auto eventTypesData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(eventTypeArray, 0));

    // return newEventTypes via arg-pointer
    const auto& vector = NativeEventReader::toEvents(size, byteData, doubleData, eventTypesData);
    dxfg_event_type_list* pEventTypeList = *ppEventTypeList;
    pEventTypeList->size = size;
    for (int i = 0; i < size; ++i) {
      pEventTypeList->elements[i] = vector[i];
    }

    env_->ReleasePrimitiveArrayCritical(byteArray, const_cast<char*>(byteData), 0);
    env_->ReleasePrimitiveArrayCritical(doubleArray, const_cast<double*>(doubleData), 0);
    env_->ReleasePrimitiveArrayCritical(eventTypeArray, const_cast<char*>(eventTypesData), 0);
    env_->DeleteLocalRef(eventTypeArray);
    env_->DeleteLocalRef(doubleArray);
    env_->DeleteLocalRef(byteArray);
    env_->DeleteLocalRef(doubleBuffer);
    env_->DeleteLocalRef(byteBuffer);
  }
}
