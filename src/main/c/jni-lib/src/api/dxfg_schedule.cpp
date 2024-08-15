// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_schedule.h"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_session_filter_t *dxfg_SessionFilter_new(graal_isolatethread_t *thread, dxfg_session_filter_function filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_filter_t *dxfg_SessionFilter_getInstance(graal_isolatethread_t *thread,
                                                      dxfg_session_filter_prepare_t filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_filter_t *dxfg_DayFilter_new(graal_isolatethread_t *thread, dxfg_day_filter_function filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_filter_t *dxfg_DayFilter_getInstance(graal_isolatethread_t *thread, dxfg_day_filter_prepare_t filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_schedule_t *dxfg_Schedule_getInstance(graal_isolatethread_t *thread,
                                           dxfg_instrument_profile_t *instrumentProfile) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_schedule_t *dxfg_Schedule_getInstance2(graal_isolatethread_t *thread, const char *scheduleDefinition) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_schedule_t *dxfg_Schedule_getInstance3(graal_isolatethread_t *thread, dxfg_instrument_profile_t *instrumentProfile,
                                            const char *venue) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_string_list *dxfg_Schedule_getTradingVenues(graal_isolatethread_t *thread,
                                                 dxfg_instrument_profile_t *instrumentProfile) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Schedule_downloadDefaults(graal_isolatethread_t *thread, const char *downloadConfig) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Schedule_setDefaults(graal_isolatethread_t *thread, const char *data, int32_t size) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_session_t *dxfg_Schedule_getSessionByTime(graal_isolatethread_t *thread, dxfg_schedule_t *schedule, int64_t time) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Schedule_getDayByTime(graal_isolatethread_t *thread, dxfg_schedule_t *schedule, int64_t time) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Schedule_getDayById(graal_isolatethread_t *thread, dxfg_schedule_t *schedule, int32_t dayId) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Schedule_getDayByYearMonthDay(graal_isolatethread_t *thread, dxfg_schedule_t *schedule,
                                               int32_t yearMonthDay) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Schedule_getNearestSessionByTime(graal_isolatethread_t *thread, dxfg_schedule_t *schedule,
                                                      int64_t time, dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Schedule_findNearestSessionByTime(graal_isolatethread_t *thread, dxfg_schedule_t *schedule,
                                                       int64_t time, dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_Schedule_getName(graal_isolatethread_t *thread, dxfg_schedule_t *schedule) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_Schedule_getTimeZone(graal_isolatethread_t *thread, dxfg_schedule_t *schedule) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_Schedule_getTimeZone_getID(graal_isolatethread_t *thread, dxfg_schedule_t *schedule) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Session_getDay(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Session_getType(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Session_isTrading(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Session_isEmpty(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_Session_getStartTime(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_Session_getEndTime(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Session_containsTime(graal_isolatethread_t *thread, dxfg_session_t *session, int64_t time) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_session_t *dxfg_Session_getPrevSession(graal_isolatethread_t *thread, dxfg_session_t *session,
                                            dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Session_getNextSession(graal_isolatethread_t *thread, dxfg_session_t *session,
                                            dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Session_findPrevSession(graal_isolatethread_t *thread, dxfg_session_t *session,
                                             dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Session_findNextSession(graal_isolatethread_t *thread, dxfg_session_t *session,
                                             dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Session_hashCode(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Session_equals(graal_isolatethread_t *thread, dxfg_session_t *session, dxfg_session_t *otherSession) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_Session_toString(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_schedule_t *dxfg_Day_getSchedule(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Day_getDayId(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_getYearMonthDay(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_getYear(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_getMonthOfYear(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_getDayOfMonth(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_getDayOfWeek(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_isHoliday(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_isShortDay(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_isTrading(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_Day_getStartTime(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_Day_getEndTime(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_containsTime(graal_isolatethread_t *thread, dxfg_day_t *day, int64_t time) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_Day_getResetTime(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_session_list *dxfg_Day_getSessions(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Day_getSessionByTime(graal_isolatethread_t *thread, dxfg_day_t *day, int64_t time) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Day_getFirstSession(graal_isolatethread_t *thread, dxfg_day_t *day,
                                         dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Day_getLastSession(graal_isolatethread_t *thread, dxfg_day_t *day, dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Day_findFirstSession(graal_isolatethread_t *thread, dxfg_day_t *day,
                                          dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_session_t *dxfg_Day_findLastSession(graal_isolatethread_t *thread, dxfg_day_t *day,
                                         dxfg_session_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Day_getPrevDay(graal_isolatethread_t *thread, dxfg_day_t *day, dxfg_day_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Day_getNextDay(graal_isolatethread_t *thread, dxfg_day_t *day, dxfg_day_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Day_findPrevDay(graal_isolatethread_t *thread, dxfg_day_t *day, dxfg_day_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_day_t *dxfg_Day_findNextDay(graal_isolatethread_t *thread, dxfg_day_t *day, dxfg_day_filter_t *filter) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Day_hashCode(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Day_equals(graal_isolatethread_t *thread, dxfg_day_t *day, dxfg_day_t *otherDay) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_Day_toString(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_Day_release(graal_isolatethread_t *thread, dxfg_day_t *day) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Session_release(graal_isolatethread_t *thread, dxfg_session_t *session) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_SessionList_wrapper_release(graal_isolatethread_t *thread, dxfg_session_list *sessions) {
    return INVALID_INT_RESULT_MINUS_ONE;
}
