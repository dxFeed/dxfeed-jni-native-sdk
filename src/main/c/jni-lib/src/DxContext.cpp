// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxContext.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {

  DxContext& dxfeed::DxContext::getInstance() {
    static DxContext instance;
    return instance;
  }

  DxContext::DxContext() :
    env_{jniEnv}
 {
   javaHelperClass_ = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JniTest;");
   addEventListenerHelperMethodId_ = jni::safeGetStaticMethodID(jniEnv, javaHelperClass_, "addEventListener",
                                                                "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
 }

  jclass DxContext::helperClass() {
    return javaHelperClass_;
  }

  jmethodID DxContext::addEventListenerMethod() {
    return addEventListenerHelperMethodId_;
  }

  void onClose(jobject clazz) {
    jniEnv->DeleteGlobalRef(clazz);
  }
}
