// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

int graal_create_isolate(graal_create_isolate_params_t* params, graal_isolate_t** isolate,
                         graal_isolatethread_t** thread);

graal_isolatethread_t* graal_get_current_thread(graal_isolate_t* isolate);

int graal_attach_thread(graal_isolate_t* isolate, graal_isolatethread_t** env);

int graal_detach_thread(graal_isolatethread_t*);

#if defined(__cplusplus)
}
#endif
#endif
