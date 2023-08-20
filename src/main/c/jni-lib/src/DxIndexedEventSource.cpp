// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxIndexedEventSource.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"

namespace dxfeed {
  using namespace jni;

  DxIndexedEventSource::DxIndexedEventSource(JNIEnv* env, const char* name) {
    auto jDxClass = safeFindClass(env, DX_INDEXED_EVENT_SOURCE_CLASS_NAME);
    const char* methodName = "newOrderSourceByName";
    const char* methodSignature = "(Ljava/lang/String;[J)Lcom/dxfeed/event/IndexedEventSource;";
    auto methodId = safeGetStaticMethodID(env, jDxClass, methodName, methodSignature);
    name_ = name;
    auto jName = env->NewStringUTF(name);
    auto data = env->NewLongArray(2);
    indexedEventSource_ = env->CallStaticObjectMethod(jDxClass, methodId, jName, data);
    env->DeleteLocalRef(jName);

    auto pData = r_cast<jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    type_ = static_cast<dxfg_indexed_event_source_type_t>(pData[0]);
    id_ = static_cast<int32_t>(pData[1]);
    env->ReleasePrimitiveArrayCritical(data, pData, 0);
    env->DeleteLocalRef(data);
    env->DeleteLocalRef(jDxClass);
  }

  DxIndexedEventSource::DxIndexedEventSource(JNIEnv* env, const int32_t sourceId) {
    auto jDxClass = safeFindClass(env, DX_INDEXED_EVENT_SOURCE_CLASS_NAME);
    const char* methodName = "newOrderSourceById";
    const char* methodSignature = "(I[J)Lcom/dxfeed/event/market/OrderSource;";
    auto methodId = safeGetStaticMethodID(env, jDxClass, methodName, methodSignature);
    auto data = env->NewLongArray(2);
    auto jIndexedEventSource = env->CallStaticObjectMethod(jDxClass, methodId, sourceId, data);
    indexedEventSource_ = env->NewGlobalRef(jIndexedEventSource);
    name_ = getName(env);
    auto jLongData = r_cast<jlong*>(env->GetPrimitiveArrayCritical(data, 0));
    type_ = static_cast<dxfg_indexed_event_source_type_t>(jLongData[0]);
    id_ = static_cast<int32_t>(jLongData[1]);
    env->ReleasePrimitiveArrayCritical(data, jLongData, 0);
    env->DeleteLocalRef(jIndexedEventSource);
    env->DeleteLocalRef(data);
    env->DeleteLocalRef(jDxClass);
  }

  const char* DxIndexedEventSource::getName(JNIEnv* env) {
    auto jEventSourceClass = env->GetObjectClass(indexedEventSource_);
    const char* methodName = "name";
    const char* methodSignature = "()Ljava/lang/String;";
    auto methodId = safeGetMethodID(env, jEventSourceClass, methodName, methodSignature);
    auto jName = r_cast<jstring>(env->CallObjectMethod(indexedEventSource_, methodId));

    jsize stringUtfLength = env->GetStringUTFLength(jName);
    auto name = new char[stringUtfLength + 1];
    name[stringUtfLength] = 0;
    const char* utfChars = env->GetStringUTFChars(jName, 0);
    memcpy(name, utfChars, stringUtfLength);
    env->ReleaseStringUTFChars(jName, utfChars);
    env->DeleteLocalRef(jName);
    env->DeleteLocalRef(jEventSourceClass);
    return name;
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
    auto jDxClass = safeFindClass(env, "Lcom/dxfeed/event/market/OrderSource;");
    const char* methodName = "isSpecialSourceId";
    const char* methodSignature = "(I)Z";
    auto methodId = safeGetStaticMethodID(env, jDxClass, methodName, methodSignature);
    auto result = env->CallStaticBooleanMethod(jDxClass, methodId, index);
    env->DeleteLocalRef(jDxClass);
    return result;
  }

  dxfg_indexed_event_source_t* DxIndexedEventSource::createByEventType(JNIEnv* env, dxfg_event_type_t* pEventType) {
    switch (pEventType->clazz) {
      case DXFG_EVENT_GREEKS:
      case DXFG_EVENT_CANDLE:
      case DXFG_EVENT_DAILY_CANDLE:
      case DXFG_EVENT_TIME_AND_SALE:
      case DXFG_EVENT_UNDERLYING:
      case DXFG_EVENT_THEO_PRICE:
      case DXFG_EVENT_SERIES: {
        return dxfg_IndexedEventSource_new(env, nullptr);
      }
      case DXFG_EVENT_ORDER_BASE:
      case DXFG_EVENT_SPREAD_ORDER:
      case DXFG_EVENT_ORDER:
      case DXFG_EVENT_ANALYTIC_ORDER: {
        const auto index = r_cast<dxfg_order_base_t*>(pEventType)->index;
        int sourceId = static_cast<int32_t>(index >> 48);
        if (!DxIndexedEventSource::isSpecialSourceId(env, sourceId)) {
          sourceId = static_cast<int32_t>(index >> 32);
        }
        return r_cast<dxfg_indexed_event_source_t*>(new dxfeed::DxIndexedEventSource(env, sourceId));
      }
      default: {
        const char* className = getEventClassType(pEventType->clazz);
        javaLogger->error("ClassCastException(\"%\" is not Class<? extends IndexedEvent>, unknown symbol type: ",
                          className);
        return nullptr;
      }
    }
  }
}