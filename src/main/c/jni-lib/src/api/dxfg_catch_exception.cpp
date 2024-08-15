// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_catch_exception.h"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_exception_t *dxfg_get_and_clear_thread_exception_t(graal_isolatethread_t *thread) {
    return dxfeed::jni::internal::dxThreadException->getAndClearThreadException(thread);
}

void dxfg_print_exception(graal_isolatethread_t *thread, dxfg_exception_t *exception) {
    if (exception) {
        printf("%s:%s\n", exception->class_name, exception->message);
        if (exception->print_stack_trace && strlen(exception->print_stack_trace) != 0) {
            printf("\tat %s\n", exception->print_stack_trace);
        }
    }
}

void dxfg_Exception_StackTrace_release(dxfg_stack_trace_element_list *stackTrace) {
    // TODO: implement

    delete stackTrace;
}

void dxfg_Exception_release(graal_isolatethread_t *, dxfg_exception_t *exception) {
    if (exception) {
        delete exception->class_name;
        delete exception->message;
        delete exception->print_stack_trace;
        dxfg_Exception_StackTrace_release(exception->stack_trace);
        dxfg_Exception_release(nullptr, exception->cause);
    }
    delete exception;
}
