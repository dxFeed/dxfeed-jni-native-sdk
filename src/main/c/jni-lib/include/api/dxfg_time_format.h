// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_API__TIME_FORMAT_H_
#define DXFEED_JNI_NATIVE_API__TIME_FORMAT_H_

#include "dxfg_javac.h"

#ifdef __cplusplus
extern "C" {
#else
#include <stdint.h>
#endif

dxfg_time_format_t*         dxfg_TimeFormat_DEFAULT(graal_isolatethread_t *thread);
dxfg_time_format_t*         dxfg_TimeFormat_GMT(graal_isolatethread_t *thread);
// TODO: implement
dxfg_time_format_t*         dxfg_TimeFormat_getInstance(graal_isolatethread_t *thread, dxfg_time_zone_t* timeZone);
dxfg_time_format_t*         dxfg_TimeFormat_withTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);
dxfg_time_format_t*         dxfg_TimeFormat_withMillis(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);
dxfg_time_format_t*         dxfg_TimeFormat_asFullIso(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);
int64_t                     dxfg_TimeFormat_parse(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat, const char* value);
const char*                 dxfg_TimeFormat_format(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat, int64_t value);
// TODO: implement
dxfg_time_zone_t*           dxfg_TimeFormat_getTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);

#ifdef __cplusplus
}
#endif

#endif //DXFEED_JNI_NATIVE_API__TIME_FORMAT_H_
