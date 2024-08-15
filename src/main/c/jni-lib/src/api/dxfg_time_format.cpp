// Copyright © 2024 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/dxfg_time_format.h"
#include "dxfeed/utils/JNIUtils.hpp"
#include "dxfeed/utils/java/DxTimeFormat.hpp"

dxfg_time_format_t *dxfg_TimeFormat_DEFAULT(graal_isolatethread_t *thread) {
    return dxfeed::r_cast<dxfg_time_format_t *>(dxfeed::jni::DxTimeFormat::getDefault());
}

dxfg_time_format_t *dxfg_TimeFormat_GMT(graal_isolatethread_t *thread) {
    return dxfeed::r_cast<dxfg_time_format_t *>(dxfeed::jni::DxTimeFormat::getGMT());
}

dxfg_time_format_t *dxfg_TimeFormat_getInstance(graal_isolatethread_t *thread, dxfg_time_zone_t *timeZone) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_format_t *dxfg_TimeFormat_withTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t *timeFormat) {
    auto pDxTimeFormat = dxfeed::r_cast<dxfeed::jni::DxTimeFormat *>(timeFormat);
    return dxfeed::r_cast<dxfg_time_format_t *>(pDxTimeFormat->withTimeZone(thread));
}

dxfg_time_format_t *dxfg_TimeFormat_withMillis(graal_isolatethread_t *thread, dxfg_time_format_t *timeFormat) {
    auto pDxTimeFormat = dxfeed::r_cast<dxfeed::jni::DxTimeFormat *>(timeFormat);
    return dxfeed::r_cast<dxfg_time_format_t *>(pDxTimeFormat->withMillis(thread));
}

dxfg_time_format_t *dxfg_TimeFormat_asFullIso(graal_isolatethread_t *thread, dxfg_time_format_t *timeFormat) {
    auto pDxTimeFormat = dxfeed::r_cast<dxfeed::jni::DxTimeFormat *>(timeFormat);
    return dxfeed::r_cast<dxfg_time_format_t *>(pDxTimeFormat->asFullIso(thread));
}

int64_t dxfg_TimeFormat_parse(graal_isolatethread_t *thread, dxfg_time_format_t *timeFormat, const char *value) {
    auto pDxTimeFormat = dxfeed::r_cast<dxfeed::jni::DxTimeFormat *>(timeFormat);
    return pDxTimeFormat->parse(thread, value);
}

const char *dxfg_TimeFormat_format(graal_isolatethread_t *thread, dxfg_time_format_t *timeFormat, int64_t value) {
    auto pDxTimeFormat = dxfeed::r_cast<dxfeed::jni::DxTimeFormat *>(timeFormat);
    return pDxTimeFormat->format(thread, value);
}

dxfg_time_zone_t *dxfg_TimeFormat_getTimeZone(graal_isolatethread_t *thread, dxfg_time_format_t *timeFormat) {
    return INVALID_PTR_RESULT_NULLPTR;
}
