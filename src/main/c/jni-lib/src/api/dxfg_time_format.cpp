// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include "api/dxfg_time_format.h"


dxfg_time_format_t* dxfg_TimeFormat_DEFAULT(graal_isolatethread_t *thread) {

}

dxfg_time_format_t* dxfg_TimeFormat_GMT(graal_isolatethread_t *thread) {

}


dxfg_time_format_t* dxfg_TimeFormat_withTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat) {

}

dxfg_time_format_t* dxfg_TimeFormat_withMillis(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat) {

}

dxfg_time_format_t* dxfg_TimeFormat_asFullIso(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat) {

}

int64_t dxfg_TimeFormat_parse(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat, const char* value) {

}

const char* dxfg_TimeFormat_format(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat, int64_t value) {

}

//dxfg_time_format_t*         dxfg_TimeFormat_getInstance(graal_isolatethread_t *thread, dxfg_time_zone_t* timeZone);

//dxfg_time_zone_t*           dxfg_TimeFormat_getTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t* timeFormat);

