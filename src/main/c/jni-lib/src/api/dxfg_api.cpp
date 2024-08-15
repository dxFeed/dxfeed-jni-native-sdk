// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include <sstream>

#include "dxfeed/DxEndpoint.hpp"
#include "dxfeed/DxEndpointBuilder.hpp"
#include "dxfeed/listeners/DxEventListener.hpp"
#include "dxfeed/listeners/DxStateChangeListener.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

graal_isolatethread_t *create_thread() {
    return create_thread_with_VMOptions(nullptr);
}

graal_isolatethread_t *create_thread_with_VMOptions(graal_create_isolate_params_t *javaVmOptions) {
    graal_isolate_t *isolate;
    graal_isolatethread_t *thread;
    int hr = graal_create_isolate(javaVmOptions, &isolate, &thread);
    return (hr == JNI_OK) ? thread : nullptr;
}

dxfg_endpoint_state_change_listener_t *
dxfg_PropertyChangeListener_new(graal_isolatethread_t *thread, dxfg_endpoint_state_change_listener_func userFunc,
                                void *userData) {
    auto *pDxStateChangeListener = dxfeed::DxStateChangeListener::create(thread, userFunc, userData);
    return dxfeed::r_cast<dxfg_endpoint_state_change_listener_t *>(pDxStateChangeListener);
}

dxfg_feed_event_listener_t *dxfg_DXFeedEventListener_new(graal_isolatethread_t *thread,
                                                         dxfg_feed_event_listener_function user_func, void *user_data) {
    auto *pDxEventListener = dxfeed::DxEventListener::create(thread, user_func, user_data);
    return dxfeed::r_cast<dxfg_feed_event_listener_t *>(pDxEventListener);
}

int dxfg_JavaObjectHandler_release(graal_isolatethread_t *thread, dxfg_java_object_handler *object) {
    if (object) {
        std::stringstream ss;
        ss << "dxfg_JavaObjectHandler_release: ";
        auto pObject = object->dxfg_java_object_handle;
        ss << "\t dxfg_java_object_handle: " << pObject;
        const auto &name = pObject ? dxfeed::jni::internal::javaLangClass->getName(thread, pObject) : "";
        if (!name.empty()) {
            ss << ", name: " << name;
            if (name == dxfeed::DxEndpoint::JAVA_CLASS_NAME) {
                dxfg_DXEndpoint_release(thread, dxfeed::r_cast<dxfg_endpoint_t *>(object));
            } else if (name == dxfeed::DxEndpointBuilder::JAVA_CLASS_NAME) {
                dxfg_DXEndpointBuilder_release(thread, dxfeed::r_cast<dxfg_endpoint_builder_t *>(object));
            } else if (name == dxfeed::DxSubscription::JAVA_SUBSCRIPTION_CLASS_NAME ||
                       name == dxfeed::DxSubscription::JAVA_TIME_SERIES_SUBSCRIPTION_NAME) {
                dxfg_DXSubscription_release(thread, dxfeed::r_cast<dxfg_subscription_t *>(object));
            } else {
                ss << ", LEAKED: " << std::hex << pObject;
            }
        } else {
            ss << ", already released: " << std::hex << pObject;
            delete object;
        }
        dxfeed::jni::javaLogger->trace(thread, ss.str());
    }
    return JNI_OK;
}

const char *dxfg_Object_toString(graal_isolatethread_t *, dxfg_java_object_handler *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Object_equals(graal_isolatethread_t *, dxfg_java_object_handler *, dxfg_java_object_handler *) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_Object_hashCode(graal_isolatethread_t *, dxfg_java_object_handler *) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_Comparable_compareTo(graal_isolatethread_t *thread, dxfg_java_object_handler *object,
                                  dxfg_java_object_handler *other) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_Object_finalize(graal_isolatethread_t *thread, dxfg_java_object_handler *handler,
                             dxfg_finalize_function finalize, void *user_data) {
    printf("dxfg_Object_finalize to nothing\n"); // todo: discuss
    return JNI_OK;
}

int dxfg_CList_JavaObjectHandler_release(graal_isolatethread_t *thread, dxfg_java_object_handler_list *list) {
    int success = JNI_OK;
    if (list && list->size > 0 && list->elements) {
        for (int i = 0; i < list->size; ++i) {
            dxfg_java_object_handler *pHandler = list->elements[i];
            if (pHandler) {
                success &= dxfg_JavaObjectHandler_release(thread, pHandler);
            }
        }
    }
    return success;
}

int32_t dxfg_String_release(graal_isolatethread_t *thread, const char *string) {
    delete string;
    return JNI_OK;
}

int32_t dxfg_CList_String_release(graal_isolatethread_t *thread, dxfg_string_list *list) {
    if (list && list->size > 0 && list->elements) {
        for (int i = 0; i < list->size; ++i) {
            delete list->elements[i];
        }
    }
    return JNI_OK;
}

dxfg_executor_t *dxfg_Executors_newFixedThreadPool(graal_isolatethread_t *, int, const char *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_executor_t *dxfg_Executors_newScheduledThreadPool(graal_isolatethread_t *, int, const char *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_executor_t *dxfg_ExecutorBaseOnConcurrentLinkedQueue_new(graal_isolatethread_t *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_ExecutorBaseOnConcurrentLinkedQueue_processAllPendingTasks(graal_isolatethread_t *, dxfg_executor_t *) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_input_stream_t *dxfg_ByteArrayInputStream_new(graal_isolatethread_t *, const char *, int32_t) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_gc(graal_isolatethread_t *) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_java_object_handler *dxfg_throw_exception(graal_isolatethread_t *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_zone_t *dxfg_TimeZone_getTimeZone(graal_isolatethread_t *, const char *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_zone_t *dxfg_TimeZone_getDefault(graal_isolatethread_t *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_TimeZone_getID(graal_isolatethread_t *, dxfg_time_zone_t *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_TimeZone_getDisplayName(graal_isolatethread_t *, dxfg_time_zone_t *) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_TimeZone_getDisplayName2(graal_isolatethread_t *, dxfg_time_zone_t *, int32_t, int32_t) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_TimeZone_getDSTSavings(graal_isolatethread_t *, dxfg_time_zone_t *) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_useDaylightTime(graal_isolatethread_t *, dxfg_time_zone_t *) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_observesDaylightTime(graal_isolatethread_t *, dxfg_time_zone_t *) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_getOffset(graal_isolatethread_t *, dxfg_time_zone_t *, int64_t) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_getOffset2(graal_isolatethread_t *, dxfg_time_zone_t *timeZone, int32_t era, int32_t year,
                                 int32_t month, int32_t day, int32_t dayOfWeek, int32_t milliseconds) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_getRawOffset(graal_isolatethread_t *thread, dxfg_time_zone_t *timeZone) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_hasSameRules(graal_isolatethread_t *thread, dxfg_time_zone_t *timeZone, dxfg_time_zone_t *other) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_inDaylightTime(graal_isolatethread_t *thread, dxfg_time_zone_t *timeZone, int64_t date) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_setID(graal_isolatethread_t *thread, dxfg_time_zone_t *timeZone, const char *ID) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int32_t dxfg_TimeZone_setRawOffset(graal_isolatethread_t *thread, dxfg_time_zone_t *timeZone, int32_t offsetMillis) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

dxfg_time_period_t *dxfg_TimePeriod_ZERO(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_period_t *dxfg_TimePeriod_UNLIMITED(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_period_t *dxfg_TimePeriod_valueOf(graal_isolatethread_t *thread, int64_t value) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_period_t *dxfg_TimePeriod_valueOf2(graal_isolatethread_t *thread, const char *value) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int64_t dxfg_TimePeriod_getTime(graal_isolatethread_t *thread, dxfg_time_period_t *timePeriod) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimePeriod_getSeconds(graal_isolatethread_t *thread, dxfg_time_period_t *timePeriod) {
    return INVALID_INT_RESULT_MINUS_MIN;
}

int64_t dxfg_TimePeriod_getNanos(graal_isolatethread_t *thread, dxfg_time_period_t *timePeriod) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_auth_token_t *dxfg_AuthToken_valueOf(graal_isolatethread_t *thread, const char *string) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_auth_token_t *dxfg_AuthToken_createBasicToken(graal_isolatethread_t *thread, const char *userPassword) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_auth_token_t *dxfg_AuthToken_createBasicToken2(graal_isolatethread_t *thread, const char *user,
                                                    const char *password) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_auth_token_t *dxfg_AuthToken_createBasicTokenOrNull(graal_isolatethread_t *thread, const char *user,
                                                         const char *password) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_auth_token_t *dxfg_AuthToken_createBearerToken(graal_isolatethread_t *thread, const char *token) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_auth_token_t *dxfg_AuthToken_createBearerTokenOrNull(graal_isolatethread_t *thread, const char *token) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_auth_token_t *dxfg_AuthToken_createCustomToken(graal_isolatethread_t *thread, const char *scheme,
                                                    const char *value) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_AuthToken_getHttpAuthorization(graal_isolatethread_t *thread, dxfg_auth_token_t *authToken) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_AuthToken_getUser(graal_isolatethread_t *thread, dxfg_auth_token_t *authToken) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_AuthToken_getPassword(graal_isolatethread_t *thread, dxfg_auth_token_t *authToken) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_AuthToken_getScheme(graal_isolatethread_t *thread, dxfg_auth_token_t *authToken) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_AuthToken_getValue(graal_isolatethread_t *thread, dxfg_auth_token_t *authToken) {
    return INVALID_PTR_RESULT_NULLPTR;
}
