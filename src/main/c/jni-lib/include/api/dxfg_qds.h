// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_API_QDS_H_
#define DXFEED_JNI_NATIVE_API_QDS_H_

#ifdef __cplusplus
extern "C" {
#include <cstdint>
#else
#    include <stdint.h>
#endif

#include "graal_isolate.h"
#include "dxfg_javac.h"

/** @defgroup Qds
 *  @{
 */
dxfg_string_list* dxfg_Tools_parseSymbols(graal_isolatethread_t* thread, const char* symbolList);
int32_t           dxfg_Tools_main(graal_isolatethread_t* thread, dxfg_string_list* args);
/** @} */ // end of QDS

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_QDS_H_




