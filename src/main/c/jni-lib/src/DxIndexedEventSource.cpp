// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxIndexedEventSource.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  using namespace jni;
  using namespace jni::internal;

  DxIndexedEventSource::DxIndexedEventSource(JNIEnv* env, const char* name) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxOrderedSource;");
    jmethodID newOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "newOrderedSource",
                                                              "(Ljava/lang/String;[J)V");
    jstring jName = env->NewStringUTF(name);
    jlongArray data = env->NewLongArray(3);
    env->CallStaticVoidMethod(dxClass, newOrderedSourceId, jName, data);
    env->DeleteLocalRef(jName);

    auto pData = r_cast <jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    type_ = static_cast<dxfg_indexed_event_source_type_t>(pData[0]);
    id_ = static_cast<int32_t>(pData[1]);
    name_ = name;
    nativeHandleId_ = pData[2];
    env->ReleasePrimitiveArrayCritical(data, pData, 0);
    env->DeleteLocalRef(data);
  }

  void DxIndexedEventSource::releaseJavaObject(graal_isolatethread_t* env) {
    jclass dxClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DxOrderedSource;");
    jmethodID releaseOrderedSourceId = jni::safeGetStaticMethodID(env, dxClass, "releaseOrderedSource", "(J)V");
    env->CallStaticVoidMethod(dxClass, releaseOrderedSourceId, nativeHandleId_);
    id_ = 0;
    name_ = nullptr;
    nativeHandleId_ = 0;
  }
}