// SPDX-License-Identifier: MPL-2.0

#ifndef __GRAAL_ISOLATE_H
#define __GRAAL_ISOLATE_H

#include <jni.h>
/*
 * Structure representing a thread that is attached to an isolate. A pointer to
 * such a structure can be passed to an entry point as the execution context,
 * requiring that the calling thread has been attached to that isolate.
 */
struct __graal_isolatethread_t;
typedef struct JNIEnv_ graal_isolatethread_t;


#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif
#endif
