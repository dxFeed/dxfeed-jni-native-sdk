// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_ipf.h"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_ipf_connection_t *dxfg_InstrumentProfileConnection_createConnection(graal_isolatethread_t *thread,
                                                                         const char *address,
                                                                         dxfg_ipf_collector_t *collector) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_InstrumentProfileConnection_getAddress(graal_isolatethread_t *thread,
                                                        dxfg_ipf_connection_t *connection) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int64_t dxfg_InstrumentProfileConnection_getUpdatePeriod(graal_isolatethread_t *thread,
                                                         dxfg_ipf_connection_t *connection) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileConnection_setUpdatePeriod(graal_isolatethread_t *thread,
                                                         dxfg_ipf_connection_t *connection, int64_t newValue) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_ipf_connection_state_t dxfg_InstrumentProfileConnection_getState(graal_isolatethread_t *thread,
                                                                      dxfg_ipf_connection_t *connection) {
    return static_cast<dxfg_ipf_connection_state_t>(INVALID_INT_RESULT_MINUS_ONE);
}

int64_t dxfg_InstrumentProfileConnection_getLastModified(graal_isolatethread_t *thread,
                                                         dxfg_ipf_connection_t *connection) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileConnection_start(graal_isolatethread_t *thread, dxfg_ipf_connection_t *connection) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileConnection_close(graal_isolatethread_t *thread, dxfg_ipf_connection_t *connection) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_ipf_connection_state_change_listener_t *
dxfg_IpfPropertyChangeListener_new(graal_isolatethread_t *thread,
                                   dxfg_ipf_connection_state_change_listener_func user_func, void *user_data) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfileConnection_addStateChangeListener(graal_isolatethread_t *thread,
                                                                dxfg_ipf_connection_t *connection,
                                                                dxfg_ipf_connection_state_change_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t
dxfg_InstrumentProfileConnection_removeStateChangeListener(graal_isolatethread_t *thread,
                                                           dxfg_ipf_connection_t *connection,
                                                           dxfg_ipf_connection_state_change_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileConnection_waitUntilCompleted(graal_isolatethread_t *thread,
                                                            dxfg_ipf_connection_t *connection, int64_t timeoutInMs) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_CList_InstrumentProfile_wrapper_release(graal_isolatethread_t *thread, dxfg_instrument_profile_list *ips) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_CList_InstrumentProfile_release(graal_isolatethread_t *thread, dxfg_instrument_profile_list *ips) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_ipf_collector_t *dxfg_InstrumentProfileCollector_new(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int64_t dxfg_InstrumentProfileCollector_getLastUpdateTime(graal_isolatethread_t *thread,
                                                          dxfg_ipf_collector_t *collector) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileCollector_updateInstrumentProfile(graal_isolatethread_t *thread,
                                                                dxfg_ipf_collector_t *collector,
                                                                dxfg_instrument_profile_t *ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileCollector_updateInstrumentProfiles(graal_isolatethread_t *thread,
                                                                 dxfg_ipf_collector_t *collector,
                                                                 dxfg_instrument_profile_list *ips,
                                                                 dxfg_java_object_handler *generation) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileCollector_removeGenerations(graal_isolatethread_t *thread,
                                                          dxfg_ipf_collector_t *collector,
                                                          dxfg_java_object_handler_list *generations) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_iterable_ip_t *dxfg_InstrumentProfileCollector_view(graal_isolatethread_t *thread,
                                                         dxfg_ipf_collector_t *collector) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_executor_t *dxfg_InstrumentProfileCollector_getExecutor(graal_isolatethread_t *thread,
                                                             dxfg_ipf_collector_t *collector) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfileCollector_setExecutor(graal_isolatethread_t *thread, dxfg_ipf_collector_t *collector,
                                                    dxfg_executor_t *executor) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileCollector_addUpdateListener(graal_isolatethread_t *thread,
                                                          dxfg_ipf_collector_t *collector,
                                                          dxfg_ipf_update_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}
int32_t dxfg_InstrumentProfileCollector_removeUpdateListener(graal_isolatethread_t *thread,
                                                             dxfg_ipf_collector_t *collector,
                                                             dxfg_ipf_update_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_Iterable_InstrumentProfile_hasNext(graal_isolatethread_t *thread, dxfg_iterable_ip_t *iterable_ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_instrument_profile_t *dxfg_Iterable_InstrumentProfile_next(graal_isolatethread_t *thread,
                                                                dxfg_iterable_ip_t *iterable_ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_ipf_update_listener_t *dxfg_InstrumentProfileUpdateListener_new(graal_isolatethread_t *thread,
                                                                     dxfg_ipf_update_listener_function user_func,
                                                                     void *user_data) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_reader_t *dxfg_InstrumentProfileReader_new(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int64_t dxfg_InstrumentProfileReader_getLastModified(graal_isolatethread_t *thread,
                                                     dxfg_instrument_profile_reader_t *reader) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfileReader_wasComplete(graal_isolatethread_t *thread,
                                                 dxfg_instrument_profile_reader_t *reader) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_instrument_profile_list *dxfg_InstrumentProfileReader_readFromFile(graal_isolatethread_t *thread,
                                                                        dxfg_instrument_profile_reader_t *reader,
                                                                        const char *address) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_list *dxfg_InstrumentProfileReader_readFromFile2(graal_isolatethread_t *thread,
                                                                         dxfg_instrument_profile_reader_t *reader,
                                                                         const char *address, const char *user,
                                                                         const char *password) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_list *dxfg_InstrumentProfileReader_readFromFile3(graal_isolatethread_t *thread,
                                                                         dxfg_instrument_profile_reader_t *reader,
                                                                         const char *address,
                                                                         dxfg_auth_token_t *token) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_InstrumentProfileReader_resolveSourceURL(graal_isolatethread_t *thread, const char *address) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_list *dxfg_InstrumentProfileReader_read2(graal_isolatethread_t *thread,
                                                                 dxfg_instrument_profile_reader_t *reader,
                                                                 dxfg_input_stream_t *is, const char *address) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_list *dxfg_InstrumentProfileReader_readCompressed(graal_isolatethread_t *thread,
                                                                          dxfg_instrument_profile_reader_t *reader,
                                                                          dxfg_input_stream_t *is) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_list *dxfg_InstrumentProfileReader_read(graal_isolatethread_t *thread,
                                                                dxfg_instrument_profile_reader_t *reader,
                                                                dxfg_input_stream_t *is) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_t *dxfg_InstrumentProfile_new(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_instrument_profile_t *dxfg_InstrumentProfile_new2(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

const char *dxfg_InstrumentProfile_getType(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setType(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                       const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getSymbol(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setSymbol(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                         const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getDescription(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setDescription(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                              const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getLocalSymbol(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setLocalSymbol(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                              const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getLocalDescription(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setLocalDescription(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                                   const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getCountry(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setCountry(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                          const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getOPOL(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setOPOL(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                       const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getExchangeData(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setExchangeData(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                               const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getExchanges(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setExchanges(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                            const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getCurrency(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setCurrency(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                           const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getBaseCurrency(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setBaseCurrency(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                               const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getCFI(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setCFI(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getISIN(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setISIN(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                       const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getSEDOL(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setSEDOL(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                        const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getCUSIP(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setCUSIP(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                        const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_getICB(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_setICB(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, int32_t value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_getSIC(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_setSIC(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, int32_t value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

double dxfg_InstrumentProfile_getMultiplier(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_DOUBLE_RESULT_NEG_INF;
}

int32_t dxfg_InstrumentProfile_setMultiplier(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                             double value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getProduct(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setProduct(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                          const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getUnderlying(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setUnderlying(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                             const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

double dxfg_InstrumentProfile_getSPC(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_DOUBLE_RESULT_NEG_INF;
}

int32_t dxfg_InstrumentProfile_setSPC(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, double value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getAdditionalUnderlyings(graal_isolatethread_t *thread,
                                                            dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setAdditionalUnderlyings(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                                        const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getMMY(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setMMY(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_getExpiration(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_setExpiration(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                             int32_t value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_getLastTrade(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_setLastTrade(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                            int32_t value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

double dxfg_InstrumentProfile_getStrike(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_DOUBLE_RESULT_NEG_INF;
}

int32_t dxfg_InstrumentProfile_setStrike(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, double value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getOptionType(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setOptionType(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                             const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getExpirationStyle(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setExpirationStyle(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                                  const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getSettlementStyle(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setSettlementStyle(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                                  const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getPriceIncrements(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setPriceIncrements(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                                  const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getTradingHours(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setTradingHours(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                               const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_InstrumentProfile_getField(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                            const char *name) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_setField(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip, const char *name,
                                        const char *value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

double dxfg_InstrumentProfile_getNumericField(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                              const char *name) {
    return INVALID_DOUBLE_RESULT_NEG_INF;
}

int32_t dxfg_InstrumentProfile_setNumericField(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                               const char *name, double value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_getDateField(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                            const char *name) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_InstrumentProfile_setDateField(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip,
                                            const char *name, int32_t value) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_string_list *dxfg_InstrumentProfile_getNonEmptyCustomFieldNames(graal_isolatethread_t *thread,
                                                                     dxfg_instrument_profile_t *ip) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_InstrumentProfile_release(graal_isolatethread_t *thread, dxfg_instrument_profile_t *ip) {
    return INVALID_INT_RESULT_MINUS_ONE;
}
