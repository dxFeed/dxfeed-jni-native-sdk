// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/dxfg_catch_exception.h"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_exception_t* dxfg_get_and_clear_thread_exception_t(graal_isolatethread_t *thread) {
  return dxfeed::jni::internal::dxThreadException->getAndClearThreadException(thread);
}

void dxfg_print_exception(graal_isolatethread_t *thread, dxfg_exception_t* exception) {
  if (exception) {
    printf("%s:%s\n", exception->className, exception->message);
    if (exception->stackTrace && strlen(exception->stackTrace) != 0) {
      printf("\tat %s\n", exception->stackTrace);
    }
  }
}

void dxfg_Exception_release(graal_isolatethread_t *, dxfg_exception_t *exception) {
  if (exception) {
    delete exception->className;
    delete exception->message;
    delete exception->stackTrace;
  }
  delete exception;
}
