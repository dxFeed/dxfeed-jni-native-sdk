// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/DxFeedContext.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {

  DxfgContext& dxfeed::DxfgContext::getInstance() {
    static DxfgContext instance;
    return instance;
  }

  DxfgContext::DxfgContext() :
    env_{jniEnv}
 {
   javaHelperClass_ = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JniTest;");
   addEventListenerHelperMethodId_ = jni::safeGetStaticMethodID(jniEnv, javaHelperClass_, "addEventListener",
                                                                "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
 }

  jclass DxfgContext::helperClass() {
    return javaHelperClass_;
  }

  jmethodID DxfgContext::addEventListenerMethod() {
    return addEventListenerHelperMethodId_;
  }

  void onClose(jobject clazz) {
    jniEnv->DeleteGlobalRef(clazz);
  }
}
