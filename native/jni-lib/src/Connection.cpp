#include "api/DxFeed.h"
#include "api/Connection.h"

namespace dxfeed {
  jobject createDxEndpoint(JNIEnv* env) {
    jclass dxEndpointClass = jni::safeFindClass(env, "Lcom/dxfeed/api/DXEndpoint;");
    jmethodID newBuilderMethodId = jni::safeGetStaticMethodID(env, dxEndpointClass, "newBuilder",
                                                          "()Lcom/dxfeed/api/DXEndpoint$Builder;");
    jobject dxEndpointBuilder = env->CallStaticObjectMethod(dxEndpointClass, newBuilderMethodId);
    jclass dxEndpointBuilderClass = env->GetObjectClass(dxEndpointBuilder);
    jmethodID buildId = jni::safeGetMethodID(env, dxEndpointBuilderClass, "build", "()Lcom/dxfeed/api/DXEndpoint;");
    return env->CallObjectMethod(dxEndpointBuilder, buildId);
  }

  jobject createDxFeedConnect(JNIEnv* env, jobject dxEndpoint, const char* address) {
    jclass dxEndpointClass = env->GetObjectClass(dxEndpoint);
    jmethodID connectMethodId = jni::safeGetMethodID(env, dxEndpointClass, "connect", "(Ljava/lang/String;)Lcom/dxfeed/api/DXEndpoint;");
    jstring addr = env->NewStringUTF(address);
    jobject dxEndpointConnected = env->CallObjectMethod(dxEndpoint, connectMethodId, addr);
    jmethodID getFeedId = jni::safeGetMethodID(env, dxEndpointClass, "getFeed", "()Lcom/dxfeed/api/DXFeed;");
    return env->CallObjectMethod(dxEndpointConnected, getFeedId);
  }

  Connection::Connection(JNIEnv* env, const std::string& address, dxfeed::OnCloseHandler onClose) :
    env_{env},
    onClose_(onClose)
  {
    endpoint_ = env->NewGlobalRef(createDxEndpoint(env));
    connection_ = env->NewGlobalRef(createDxFeedConnect(env, endpoint_, address.c_str()));
  }

  Subscription* Connection::createSubscription(EventType eventType) {
    return new Subscription(env_, connection_, eventType, onClose_);
  }

  Connection::~Connection() {
    onClose_(endpoint_);
    onClose_(connection_);
  }
}