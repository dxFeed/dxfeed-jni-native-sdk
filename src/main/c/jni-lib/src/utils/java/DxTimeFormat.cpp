// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/utils/java/DxTimeFormat.hpp"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/DxSymbol.hpp"

namespace dxfeed::jni {
  DxTimeFormat::DxTimeFormat(JNIEnv* env, jobject tf) {
    timeFormat_ = env->NewGlobalRef(tf);
  }

  DxTimeFormat::~DxTimeFormat() {
    dxfeed::jni::internal::jniEnv->DeleteGlobalRef(timeFormat_);
  }

  void DxTimeFormat::init(JNIEnv* env) {
    static std::mutex mutex_;
    std::unique_lock lock(mutex_);
    auto dxTFClazz = safeFindClass(env, "com/devexperts/util/TimeFormat");
    javaLogger->trace(env, "com.devexperts.util.TimeFormat: %", dxTFClazz);
    if (DEFAULT_ == nullptr && GMT_ == nullptr) {
      withTimeZoneMethodId_ = safeGetMethodID(env, dxTFClazz, "withTimeZone", "()Lcom/devexperts/util/TimeFormat;");
      withMillisMethodId_ = safeGetMethodID(env, dxTFClazz, "withMillis", "()Lcom/devexperts/util/TimeFormat;");
      asFullIsoMethodId_ = safeGetMethodID(env, dxTFClazz, "asFullIso", "()Lcom/devexperts/util/TimeFormat;");

      parseMethodId_ = safeGetMethodID(env, dxTFClazz, "parse", "(Ljava/lang/String;)Ljava/util/Date;");
      formatMethodId_ = safeGetMethodID(env, dxTFClazz, "format", "(J)Ljava/lang/String;");

      // long java.util.Date::getTime();
      auto javaDateClazz = safeFindClass(env, "java/util/Date");
      javaLogger->trace(env, "java.util.Date: %", javaDateClazz);
      dataGetTimeId_ = safeGetMethodID(env, javaDateClazz, "getTime", "()J");

      DEFAULT_ = new DxTimeFormat(env, initDefault(env, dxTFClazz));
      GMT_ = new DxTimeFormat(env, initGMT(env, dxTFClazz));
    } else {
      javaLogger->error(env, "DxTimeFormat has already initialized!");
    }
    env->DeleteLocalRef(dxTFClazz);
  }

  jobject DxTimeFormat::initDefault(JNIEnv* env, jclass dxTimeFormatClazz) {
    auto defaultFiledId = safeGetStaticFieldID(env, dxTimeFormatClazz, "DEFAULT", "Lcom/devexperts/util/TimeFormat;");
    return env->GetStaticObjectField(dxTimeFormatClazz, defaultFiledId);
  }

  jobject DxTimeFormat::initGMT(JNIEnv* env, jclass dxTimeFormatClazz) {
    auto defaultFiledId = safeGetStaticFieldID(env, dxTimeFormatClazz, "GMT", "Lcom/devexperts/util/TimeFormat;");
    return env->GetStaticObjectField(dxTimeFormatClazz, defaultFiledId);
  }

  DxTimeFormat* DxTimeFormat::getDefault() {
    return DEFAULT_;
  }

  DxTimeFormat* DxTimeFormat::getGMT() {
    return GMT_;
  }

  DxTimeFormat* DxTimeFormat::withTimeZone(JNIEnv* env) {
    jobject tfWithTimeZone = checkedCallObjectMethod(env, timeFormat_, withTimeZoneMethodId_);
    return new DxTimeFormat(env, tfWithTimeZone);
  }

  DxTimeFormat* DxTimeFormat::withMillis(JNIEnv* env) {
    jobject tfWithMillis = checkedCallObjectMethod(env, timeFormat_, withMillisMethodId_);
    return new DxTimeFormat(env, tfWithMillis);
  }

  DxTimeFormat* DxTimeFormat::asFullIso(JNIEnv* env) {
    jobject tfFullIso = checkedCallObjectMethod(env, timeFormat_, asFullIsoMethodId_);
    return new DxTimeFormat(env, tfFullIso);
  }

  int64_t DxTimeFormat::parse(JNIEnv* env, const char* value) {
    auto jStr = env->NewStringUTF(value);
    jobject javaDate = checkedCallObjectMethod(env, timeFormat_, parseMethodId_, jStr);
    jlong result = checkedCallLongMethod(env, javaDate, dataGetTimeId_);
    env->DeleteLocalRef(javaDate);
    env->DeleteLocalRef(jStr);
    return result;
  }

  const char* DxTimeFormat::format(JNIEnv* env, jlong time) {
    auto jFormatStr = checkedCallObjectMethodAndCastTo<jstring>(env, timeFormat_, formatMethodId_, time);
    return jStringToUTF8(env, jFormatStr);
  }
}
