// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef DXFEED_JNI_NATIVE_API_TIME_FORMAT_H_
#define DXFEED_JNI_NATIVE_API_TIME_FORMAT_H_

#include "dxfg_javac.h"

#ifdef __cplusplus
extern "C" {
#include <cstdint>
#else
#include <stdint.h>
#endif

typedef struct dxfg_time_format_t {
  dxfg_java_object_handler handler;
} dxfg_time_format_t;

typedef struct dxfg_time_zone_t {
  dxfg_java_object_handler handler;
} dxfg_time_zone_t;

typedef struct dxfg_time_period_t {
  dxfg_java_object_handler handler;
} dxfg_time_period_t;

dxfg_time_format_t*         dxfg_TimeFormat_DEFAULT(graal_isolatethread_t *thread);
dxfg_time_format_t*         dxfg_TimeFormat_GMT(graal_isolatethread_t *thread);
dxfg_time_format_t*         dxfg_TimeFormat_getInstance(graal_isolatethread_t *thread, dxfg_time_zone_t* timeZone);
dxfg_time_format_t*         dxfg_TimeFormat_withTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);
dxfg_time_format_t*         dxfg_TimeFormat_withMillis(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);
dxfg_time_format_t*         dxfg_TimeFormat_asFullIso(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);
int64_t                     dxfg_TimeFormat_parse(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat, const char* value);
const char*                 dxfg_TimeFormat_format(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat, int64_t value);
dxfg_time_zone_t*           dxfg_TimeFormat_getTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);


dxfg_time_zone_t*           dxfg_TimeZone_getTimeZone(graal_isolatethread_t* thread, const char* ID);
dxfg_time_zone_t*           dxfg_TimeZone_getDefault(graal_isolatethread_t* thread);
const char*                 dxfg_TimeZone_getID(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone);
const char*                 dxfg_TimeZone_getDisplayName(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone);
const char*                 dxfg_TimeZone_getDisplayName2(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, int32_t daylight, int32_t style);
int32_t                     dxfg_TimeZone_getDSTSavings(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone);
int32_t                     dxfg_TimeZone_useDaylightTime(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone);
int32_t                     dxfg_TimeZone_observesDaylightTime(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone);
int32_t                     dxfg_TimeZone_getOffset(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, int64_t date);
int32_t                     dxfg_TimeZone_getOffset2(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, int32_t era, int32_t year, int32_t month, int32_t day, int32_t dayOfWeek, int32_t milliseconds);
int32_t                     dxfg_TimeZone_getRawOffset(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone);
int32_t                     dxfg_TimeZone_hasSameRules(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, dxfg_time_zone_t* other);
int32_t                     dxfg_TimeZone_inDaylightTime(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, int64_t date);
int32_t                     dxfg_TimeZone_setID(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, const char* ID);
int32_t                     dxfg_TimeZone_setRawOffset(graal_isolatethread_t* thread, dxfg_time_zone_t* timeZone, int32_t offsetMillis);
dxfg_time_period_t*         dxfg_TimePeriod_ZERO(graal_isolatethread_t* thread);
dxfg_time_period_t*         dxfg_TimePeriod_UNLIMITED(graal_isolatethread_t* thread);
dxfg_time_period_t*         dxfg_TimePeriod_valueOf(graal_isolatethread_t* thread, int64_t value);
dxfg_time_period_t*         dxfg_TimePeriod_valueOf2(graal_isolatethread_t* thread, const char* value);
int64_t                     dxfg_TimePeriod_getTime(graal_isolatethread_t* thread, dxfg_time_period_t* timePeriod);
int32_t                     dxfg_TimePeriod_getSeconds(graal_isolatethread_t* thread, dxfg_time_period_t* timePeriod);
int64_t                     dxfg_TimePeriod_getNanos(graal_isolatethread_t* thread, dxfg_time_period_t* timePeriod);

#ifdef __cplusplus
}
#endif

#endif //DXFEED_JNI_NATIVE_API_TIME_FORMAT_H_
