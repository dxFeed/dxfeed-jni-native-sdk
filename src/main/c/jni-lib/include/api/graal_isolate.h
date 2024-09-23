// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef __GRAAL_ISOLATE_H
#define __GRAAL_ISOLATE_H

#include <jni.h>

/*
 * Structure representing a thread that is attached to an isolate. A pointer to
 * such a structure can be passed to an entry point as the execution context,
 * requiring that the calling thread has been attached to that isolate.
 */
typedef struct JNIEnv_ graal_isolatethread_t;
typedef void graal_isolate_t;
typedef void graal_create_isolate_params_t;

#if defined(__cplusplus)
extern "C" {
#endif

int graal_create_isolate(graal_create_isolate_params_t *params, graal_isolate_t **isolate,
                         graal_isolatethread_t **thread);

graal_isolatethread_t *graal_get_current_thread(graal_isolate_t *isolate);

int graal_attach_thread(graal_isolate_t *isolate, graal_isolatethread_t **env);

// TODO: implement
graal_isolate_t *graal_get_isolate(graal_isolatethread_t *thread);

int graal_detach_thread(graal_isolatethread_t *);

// TODO: implement
int graal_tear_down_isolate(graal_isolatethread_t *isolateThread);

// TODO: implement
int graal_detach_all_threads_and_tear_down_isolate(graal_isolatethread_t *isolateThread);

#if defined(__cplusplus)
}
#endif
#endif
