// SPDX-License-Identifier: MPL-2.0

#include <stdexcept>

#include "dxfeed/DxFeed.hpp"
#include "dxfeed/Connection.hpp"
#include "dxfeed/utils/LoadLibrary.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed {
  void dxfeed::DxFeed::initJavaVM(const char* javaHome, const char** consoleVmArgs, const int vmArgsCount) {
    dxfeed::internal::loadJavaVM(javaHome, consoleVmArgs, vmArgsCount);
  }

  DxFeed& dxfeed::DxFeed::getInstance() {
    static DxFeed instance;
    return instance;
  }

  DxFeed::DxFeed() :
    env_{jniEnv}
 {
   javaHelperClass_ = jni::safeFindClass(jniEnv, "Lcom/dxfeed/api/JniTest;");
   addEventListenerHelperMethodId_ = jni::safeGetStaticMethodID(jniEnv, javaHelperClass_, "addEventListener",
                                                                "(Lcom/dxfeed/api/DXFeedSubscription;J)V");
 }

  jclass DxFeed::helperClass() {
    return javaHelperClass_;
  }

  jmethodID DxFeed::addEventListenerMethod() {
    return addEventListenerHelperMethodId_;
  }

  void DxFeed::onClose(jobject clazz) {
    jniEnv->DeleteGlobalRef(clazz);
  }

  Connection* DxFeed::createConnection(const std::string& address) {
    return new Connection(env_, address, onClose);
  }
}
