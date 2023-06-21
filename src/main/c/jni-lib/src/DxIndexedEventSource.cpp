// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxIndexedEventSource.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  using namespace jni;
  using namespace jni::internal;

  DxIndexedEventSource::DxIndexedEventSource(JNIEnv* env, const char* name) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxIndexedEventSource;");
    jmethodID newOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "newOrderSourceByName",
                                                              "(Ljava/lang/String;[J)Lcom/dxfeed/event/IndexedEventSource;");
    name_ = name;
    jstring jName = env->NewStringUTF(name);
    jlongArray data = env->NewLongArray(2);
    indexedEventSource_ = env->CallStaticObjectMethod(dxClass, newOrderedSourceId, jName, data);
    env->DeleteLocalRef(jName);

    auto pData = r_cast<jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    type_ = static_cast<dxfg_indexed_event_source_type_t>(pData[0]);
    id_ = static_cast<int32_t>(pData[1]);
    env->ReleasePrimitiveArrayCritical(data, pData, 0);
    env->DeleteLocalRef(data);
  }

  DxIndexedEventSource::DxIndexedEventSource(JNIEnv* env, const int32_t sourceId) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxIndexedEventSource;");
    jmethodID newOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "newOrderSourceById",
                                                              "(I[J)Lcom/dxfeed/event/market/OrderSource;");

    jlongArray data = env->NewLongArray(2);
    indexedEventSource_ = env->NewGlobalRef(
        env->CallStaticObjectMethod(dxClass, newOrderedSourceId, sourceId, data)
    );

    jclass eventSourceClass = env->GetObjectClass(indexedEventSource_);
    jmethodID nameId = jni::safeGetStaticMethodID(env, eventSourceClass, "name", "()Ljava/lang/String;");
    auto jName = r_cast<jstring>(env->CallObjectMethod(indexedEventSource_, nameId));
    env->DeleteLocalRef(eventSourceClass);

    jsize stringUtfLength = env->GetStringUTFLength(jName);
    auto name = new char[stringUtfLength + 1];
    name_ = name;
    name[stringUtfLength] = 0;
    const char* utfChars = env->GetStringUTFChars(jName, 0);
    strcpy_s(name, stringUtfLength, utfChars);
    env->ReleaseStringUTFChars(jName, utfChars);
    env->DeleteLocalRef(jName);


    auto pData = r_cast<jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    type_ = static_cast<dxfg_indexed_event_source_type_t>(pData[0]);
    id_ = static_cast<int32_t>(pData[1]);
    env->ReleasePrimitiveArrayCritical(data, pData, 0);
    env->DeleteLocalRef(data);
  }

  DxIndexedEventSource::~DxIndexedEventSource() {
    indexedEventSource_ = nullptr;
    id_ = 0;
    delete name_;
    name_ = nullptr;
  }

  jobject DxIndexedEventSource::javaObject() const {
    return indexedEventSource_;
  }

  void DxIndexedEventSource::release(JNIEnv* env) {
    env->DeleteGlobalRef(indexedEventSource_);
    delete this;
  }

  bool DxIndexedEventSource::isSpecialSourceId(JNIEnv* env, int32_t index) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/event/market/OrderSource;");
    jmethodID isSpecialSourceIdId = jni::safeGetStaticMethodID(env, dxClass, "isSpecialSourceId", "(I)Z");
    jboolean result = env->CallStaticBooleanMethod(dxClass, isSpecialSourceIdId, index);
    return result;
  }
}