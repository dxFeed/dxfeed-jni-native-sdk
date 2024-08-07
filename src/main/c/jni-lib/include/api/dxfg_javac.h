// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_API_JAVAC_H_
#define DXFEED_JNI_NATIVE_API_JAVAC_H_

#ifdef __cplusplus
extern "C" {
#    include <cstdint>
#else
#    include <stdint.h>
#endif

#include "graal_isolate.h"

/** @defgroup Javac
 *  @{
 */

typedef struct dxfg_java_object_handler {
  jobject dxfg_java_object_handle;
} dxfg_java_object_handler;

typedef struct dxfg_java_object_handler_list {
  int32_t size;
  dxfg_java_object_handler** elements;
} dxfg_java_object_handler_list;

typedef struct dxfg_string_list {
    int32_t size;
    const char **elements;
} dxfg_string_list;

/* todo: implement later
  typedef struct dxfg_list {
    int32_t size;
    void** elements;
  } dxfg_list;

  typedef struct dxfg_executor_t {
      dxfg_java_object_handler handler;
  } dxfg_executor_t;

  typedef struct dxfg_input_stream_t {
      dxfg_java_object_handler handler;
  } dxfg_input_stream_t;
*/

typedef struct dxfg_time_format_t {
  dxfg_java_object_handler handler;
} dxfg_time_format_t;

graal_isolatethread_t* create_thread();
graal_isolatethread_t* create_thread_with_VMOptions(graal_create_isolate_params_t* javaVmOptions);

typedef void (* dxfg_finalize_function)(graal_isolatethread_t* thread, void* user_data);

int32_t dxfg_Object_finalize(graal_isolatethread_t* thread, dxfg_java_object_handler* handler,
                             dxfg_finalize_function finalize, void* user_data);

// free the memory occupied by the с data structure and release the reference to the java object
int dxfg_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler*);

// free the memory occupied by the с data structure (list and all elements) and release the reference to the java object for all elements
int dxfg_CList_JavaObjectHandler_release(graal_isolatethread_t* thread, dxfg_java_object_handler_list*);

int32_t dxfg_String_release(graal_isolatethread_t *thread, const char* string);
int32_t dxfg_CList_String_release(graal_isolatethread_t *thread, dxfg_string_list* list);

/* todo: implement later
// read the "Threads and locks" sections at https://docs.dxfeed.com/dxfeed/api/com/dxfeed/api/DXFeedSubscription.html
dxfg_executor_t*  dxfg_Executors_newFixedThreadPool(graal_isolatethread_t *thread, int nThreads, const char* nameThreads);
dxfg_executor_t*  dxfg_Executors_newScheduledThreadPool(graal_isolatethread_t *thread, int nThreads, const char* nameThreads);
dxfg_executor_t*  dxfg_ExecutorBaseOnConcurrentLinkedQueue_new(graal_isolatethread_t *thread);
int32_t           dxfg_ExecutorBaseOnConcurrentLinkedQueue_processAllPendingTasks(graal_isolatethread_t *thread, dxfg_executor_t *executor);

dxfg_input_stream_t*   dxfg_ByteArrayInputStream_new(graal_isolatethread_t *thread, const char* bytes, int32_t size);

int32_t           dxfg_gc(graal_isolatethread_t *thread); // only for testing
dxfg_java_object_handler*  dxfg_throw_exception(graal_isolatethread_t *thread); // only for testing
*/

/** @} */ // end of Javac

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_JAVAC_H_




