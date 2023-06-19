// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxOrderSource.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  using namespace jni;
  using namespace jni::internal;

  DxOrderSource::DxOrderSource(JNIEnv* env, const char* name) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxOrderSource;");
    jmethodID newOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "newOrderSourceByName",
                                                              "(Ljava/lang/String;[J)V");
    name_ = name;
    jstring jName = env->NewStringUTF(name);
    jlongArray data = env->NewLongArray(3);
    env->CallStaticVoidMethod(dxClass, newOrderedSourceId, jName, data);
    env->DeleteLocalRef(jName);

    auto pData = r_cast <jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    type_ = static_cast<dxfg_indexed_event_source_type_t>(pData[0]);
    id_ = static_cast<int32_t>(pData[1]);
    nativeHandleId_ = pData[2];
    env->ReleasePrimitiveArrayCritical(data, pData, 0);
    env->DeleteLocalRef(data);
  }

  DxOrderSource::DxOrderSource(dxfg_indexed_event_source_type_t type, int32_t id, const char* name,
                               jlong nativeHandleId):
                                             type_(type),
                                             id_(id),
                                             name_(name),
                                             nativeHandleId_(nativeHandleId)
  {}

  void DxOrderSource::releaseJavaObject(graal_isolatethread_t* env) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxOrderSource;");
    jmethodID releaseOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "releaseOrderSource", "(J)V");
    env->CallStaticVoidMethod(dxClass, releaseOrderedSourceId, nativeHandleId_);
    id_ = 0;
    name_ = nullptr;
    nativeHandleId_ = 0;
  }

  bool DxOrderSource::isSpecialSourceId(JNIEnv* env, int32_t index) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/event/market/OrderSource;");
    jmethodID isSpecialSourceIdId = jni::safeGetStaticMethodID(env, dxClass, "isSpecialSourceId", "(I)Z");
    jboolean result = env->CallStaticBooleanMethod(dxClass, isSpecialSourceIdId, index);
    return result;
  }

  dxfg_indexed_event_source_t* DxOrderSource::createOrderSourceById(JNIEnv* env, const int32_t sourceId) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxOrderSource;");
    jmethodID newOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "newOrderSourceById",
                                                              "(I[J)Ljava/lang/String;");
    jlongArray data = env->NewLongArray(3);
    auto jName = r_cast<jstring>(env->CallStaticObjectMethod(dxClass, newOrderedSourceId, sourceId, data));

    auto pData = r_cast<jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    auto type = static_cast<dxfg_indexed_event_source_type_t>(pData[0]);
    auto id = static_cast<int32_t>(pData[1]);
    auto nativeHandleId = pData[2];

    jsize stringUtfLength = env->GetStringUTFLength(jName);
    auto name = new char[stringUtfLength + 1];
    name[stringUtfLength] = 0;
    const char* utfChars = env->GetStringUTFChars(jName, 0);
    strcpy(name, utfChars);
    env->ReleaseStringUTFChars(jName, utfChars);
    env->DeleteLocalRef(jName);

    env->ReleasePrimitiveArrayCritical(data, pData, 0);
    env->DeleteLocalRef(data);
    return r_cast<dxfg_indexed_event_source_t*>(new DxOrderSource(type, id, name, nativeHandleId));
  }

}