// SPDX-License-Identifier: MPL-2.0

#include <jni.h>

#include "dxfeed/DxIndexedEventSource.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxFeed.hpp"

namespace dxfeed {
  using namespace jni;

  DxIndexedEventSource::DxIndexedEventSource(JNIEnv* env, const char* name) {
    jclass dxClass = safeFindClass(env, "Lcom/dxfeed/api/DxIndexedEventSource;");
    jmethodID newOrderedSourceId = safeGetStaticMethodID(env, dxClass, "newOrderSourceByName",
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
    jclass dxClass = safeFindClass(env, "Lcom/dxfeed/api/DxIndexedEventSource;");
    jmethodID newOrderedSourceId = safeGetStaticMethodID(env, dxClass, "newOrderSourceById",
                                                              "(I[J)Lcom/dxfeed/event/market/OrderSource;");

    jlongArray data = env->NewLongArray(2);
    indexedEventSource_ = env->NewGlobalRef(
        env->CallStaticObjectMethod(dxClass, newOrderedSourceId, sourceId, data)
    );

    jclass eventSourceClass = env->GetObjectClass(indexedEventSource_);
    jmethodID nameId = safeGetStaticMethodID(env, eventSourceClass, "name", "()Ljava/lang/String;");
    auto jName = r_cast<jstring>(env->CallObjectMethod(indexedEventSource_, nameId));
    env->DeleteLocalRef(eventSourceClass);

    jsize stringUtfLength = env->GetStringUTFLength(jName);
    auto name = new char[stringUtfLength + 1];
    name_ = name;
    name[stringUtfLength] = 0;
    const char* utfChars = env->GetStringUTFChars(jName, 0);
    memcpy(name, utfChars, stringUtfLength);
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
    jclass dxClass = safeFindClass(env, "Lcom/dxfeed/event/market/OrderSource;");
    jmethodID isSpecialSourceIdId = safeGetStaticMethodID(env, dxClass, "isSpecialSourceId", "(I)Z");
    jboolean result = env->CallStaticBooleanMethod(dxClass, isSpecialSourceIdId, index);
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