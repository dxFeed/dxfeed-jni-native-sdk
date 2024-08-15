// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_ondemand.h"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_on_demand_service_t *dxfg_OnDemandService_getInstance(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_on_demand_service_t *dxfg_OnDemandService_getInstance2(graal_isolatethread_t *thread, dxfg_endpoint_t *endpoint) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_endpoint_t *dxfg_OnDemandService_getEndpoint(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_OnDemandService_isReplaySupported(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_isReplay(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_isClear(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_OnDemandService_getTime(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

double dxfg_OnDemandService_getSpeed(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_DOUBLE_RESULT_NEG_INF;
}

int32_t dxfg_OnDemandService_replay(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service, int64_t time) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_replay2(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service, int64_t time,
                                     double speed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_pause(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_stopAndResume(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_stopAndClear(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OnDemandService_setSpeed(graal_isolatethread_t *thread, dxfg_on_demand_service_t *service, double speed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}
