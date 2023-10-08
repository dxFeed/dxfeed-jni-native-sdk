// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/ByteWriter.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/NativeEventsList.hpp"

namespace dxfeed::jni {
  NativeEventsList::NativeEventsList(JNIEnv* env):
    env_(env),
    dxNativeEventsListClass_(safeFindClass(env, "com/dxfeed/api/NativeEventsList")),
    dxByteBuffer_(safeFindClass(env, "com/dxfeed/api/buffers/ByteBuffer")),
    dxDoubleBuffer_(safeFindClass(env, "com/dxfeed/api/buffers/DoubleBuffer"))
  {
    javaLogger->info("com.dxfeed.api.NativeEventsList: %", dxNativeEventsListClass_);
    byteBuffer_ = safeGetFieldID(env, dxNativeEventsListClass_, "pBytes", "Lcom/dxfeed/api/buffers/ByteBuffer;");
    doubleBuffer_ = safeGetFieldID(env, dxNativeEventsListClass_, "pDoubles", "Lcom/dxfeed/api/buffers/DoubleBuffer;");
    pEventTypes_ = safeGetFieldID(env, dxNativeEventsListClass_, "pEventTypes", "[B");

    toByteData_ = safeGetMethodID(env_, dxByteBuffer_, "toByteData", "()[B");
    toDoubleData_ = safeGetMethodID(env_, dxDoubleBuffer_, "toDoubleData", "()[D");

    auto jDxFeedJniClazz = safeFindClass(env, NATIVE_EVENTS_LIST_JNI_CLASS_NAME);
    const char* methodName = "toList";
    const char* methodSignature = "([B[D[B)Ljava/util/List;";
    toList_ = safeGetStaticMethodID(env, jDxFeedJniClazz, methodName, methodSignature);
    env_->DeleteLocalRef(jDxFeedJniClazz);
  }

  NativeEventsList::~NativeEventsList() {
    env_->DeleteLocalRef(dxDoubleBuffer_);
    env_->DeleteLocalRef(dxByteBuffer_);
    env_->DeleteLocalRef(dxNativeEventsListClass_);

    toList_ = nullptr;
    toDoubleData_ = nullptr;
    toByteData_ = nullptr;
    doubleBuffer_ = nullptr;
    byteBuffer_ = nullptr;
    pEventTypes_ = nullptr;
    env_ = nullptr;
  }

  void NativeEventsList::toNativeEvent(jobject nativeEventsList, dxfg_event_type_t** ppEventType) {
    auto byteBuffer = env_->GetObjectField(nativeEventsList, byteBuffer_);
    auto doubleBuffer = env_->GetObjectField(nativeEventsList, doubleBuffer_);

    auto byteArray = r_cast<jbyteArray>(env_->CallObjectMethod(byteBuffer, toByteData_));
    auto doubleArray = r_cast<jbyteArray>(env_->CallObjectMethod(doubleBuffer, toDoubleData_));
    auto eventTypeArray = r_cast<jdoubleArray>(env_->GetObjectField(nativeEventsList, pEventTypes_));
    jint size = env_->GetArrayLength(eventTypeArray);

    auto byteData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(byteArray, 0));
    auto doubleData = r_cast<const double*>(env_->GetPrimitiveArrayCritical(doubleArray, 0));
    auto eventTypesData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(eventTypeArray, 0));

    // return newEventType via arg-pointer
    dxfeed::jni::ByteReader reader(size, byteData, doubleData, eventTypesData);
    auto events = reader.toEvents();
    dxfg_event_type_t* pType = events[0];
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

  void NativeEventsList::toNativeEventsList(jobject nativeEventsList, dxfg_event_type_list** ppEventTypeList) {
    auto byteBuffer = env_->GetObjectField(nativeEventsList, byteBuffer_);
    auto doubleBuffer = env_->GetObjectField(nativeEventsList, doubleBuffer_);

    auto byteArray = r_cast<jbyteArray>(env_->CallObjectMethod(byteBuffer, toByteData_));
    auto doubleArray = r_cast<jbyteArray>(env_->CallObjectMethod(doubleBuffer, toDoubleData_));
    auto eventTypeArray = r_cast<jdoubleArray>(env_->GetObjectField(nativeEventsList, pEventTypes_));
    jint size = env_->GetArrayLength(eventTypeArray);

    auto byteData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(byteArray, 0));
    auto doubleData = r_cast<const double*>(env_->GetPrimitiveArrayCritical(doubleArray, 0));
    auto eventTypesData = r_cast<const char*>(env_->GetPrimitiveArrayCritical(eventTypeArray, 0));

    // return newEventTypes via arg-pointer
    dxfeed::jni::ByteReader reader(size, byteData, doubleData, eventTypesData);
    auto events = reader.toEvents();
    dxfg_event_type_list* pEventTypeList = *ppEventTypeList;
    pEventTypeList->size = size;
    for (int i = 0; i < size; ++i) {
      pEventTypeList->elements[i] = events[i];
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

  jobject NativeEventsList::fromNativeEventsList(dxfg_event_type_list* pList) {
    auto size = pList->size;
    ByteWriter writer(size);
    for (int i = 0; i < size; ++i) {
      writer.writeEvent(pList->elements[i]);
    }

    auto jDxFeedJniClazz = safeFindClass(env_, NATIVE_EVENTS_LIST_JNI_CLASS_NAME);
    auto jByteData = writer.byteData(env_);
    auto jDoubleData = writer.doubleData(env_);
    auto jEventTypes = writer.eventTypes(env_);

    auto result = env_->CallStaticObjectMethod(jDxFeedJniClazz, toList_, jByteData, jDoubleData, jEventTypes);

    env_->DeleteLocalRef(jEventTypes);
    env_->DeleteLocalRef(jDoubleData);
    env_->DeleteLocalRef(jByteData);
    env_->DeleteLocalRef(jDxFeedJniClazz);

    return result;
  }

}
