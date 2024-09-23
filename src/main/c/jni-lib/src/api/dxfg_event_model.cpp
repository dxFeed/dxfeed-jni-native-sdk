// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_event_model.h"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_order_book_model_t *dxfg_OrderBookModel_new(graal_isolatethread_t *thread) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_indexed_event_model_t *dxfg_IndexedEventModel_new(graal_isolatethread_t *thread, dxfg_event_clazz_t clazz) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_time_series_event_model_t *dxfg_TimeSeriesEventModel_new(graal_isolatethread_t *thread, dxfg_event_clazz_t clazz) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_OrderBookModel_attach(graal_isolatethread_t *thread, dxfg_order_book_model_t *model, dxfg_feed_t *feed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_detach(graal_isolatethread_t *thread, dxfg_order_book_model_t *model, dxfg_feed_t *feed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_close(graal_isolatethread_t *thread, dxfg_order_book_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_executor_t *dxfg_OrderBookModel_getExecutor(graal_isolatethread_t *thread, dxfg_order_book_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_OrderBookModel_setExecutor(graal_isolatethread_t *thread, dxfg_order_book_model_t *model,
                                        dxfg_executor_t *executor) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_clear(graal_isolatethread_t *thread, dxfg_order_book_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_IndexedEventModel_attach(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model,
                                      dxfg_feed_t *feed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_IndexedEventModel_detach(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model,
                                      dxfg_feed_t *feed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_IndexedEventModel_close(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_executor_t *dxfg_IndexedEventModel_getExecutor(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_IndexedEventModel_setExecutor(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model,
                                           dxfg_executor_t *executor) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_IndexedEventModel_clear(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_attach(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model,
                                         dxfg_feed_t *feed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_detach(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model,
                                         dxfg_feed_t *feed) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_close(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_executor_t *dxfg_TimeSeriesEventModel_getExecutor(graal_isolatethread_t *thread,
                                                       dxfg_time_series_event_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_TimeSeriesEventModel_setExecutor(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model,
                                              dxfg_executor_t *executor) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_clear(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_getFilter(graal_isolatethread_t *thread, dxfg_order_book_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_setFilter(graal_isolatethread_t *thread, dxfg_order_book_model_t *model,
                                      dxfg_order_book_model_filter_t filter) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

const char *dxfg_OrderBookModel_getSymbol(graal_isolatethread_t *thread, dxfg_order_book_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_OrderBookModel_setSymbol(graal_isolatethread_t *thread, dxfg_order_book_model_t *model,
                                      const char *symbol) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_symbol_t *dxfg_IndexedEventModel_getSymbol(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_IndexedEventModel_setSymbol(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model,
                                         dxfg_symbol_t *symbol) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_symbol_t *dxfg_TimeSeriesEventModel_getSymbol(graal_isolatethread_t *thread,
                                                   dxfg_time_series_event_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_TimeSeriesEventModel_setSymbol(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model,
                                            dxfg_symbol_t *symbol) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_getLotSize(graal_isolatethread_t *thread, dxfg_order_book_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_setLotSize(graal_isolatethread_t *thread, dxfg_order_book_model_t *model, int32_t lotSize) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_IndexedEventModel_getSizeLimit(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_IndexedEventModel_setSizeLimit(graal_isolatethread_t *thread, dxfg_indexed_event_model_t *model,
                                            int32_t sizeLimit) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_getSizeLimit(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_setSizeLimit(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model,
                                               int32_t sizeLimit) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int64_t dxfg_TimeSeriesEventModel_getFromTime(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_TimeSeriesEventModel_setFromTime(graal_isolatethread_t *thread, dxfg_time_series_event_model_t *model,
                                              int64_t fromTime) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_observable_list_model_t *dxfg_OrderBookModel_getBuyOrders(graal_isolatethread_t *thread,
                                                               dxfg_order_book_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_observable_list_model_t *dxfg_OrderBookModel_getSellOrders(graal_isolatethread_t *thread,
                                                                dxfg_order_book_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_observable_list_model_t *dxfg_IndexedEventModel_getEventsList(graal_isolatethread_t *thread,
                                                                   dxfg_indexed_event_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_observable_list_model_t *dxfg_TimeSeriesEventModel_getEventsList(graal_isolatethread_t *thread,
                                                                      dxfg_time_series_event_model_t *model) {
    return INVALID_PTR_RESULT_NULLPTR;
}

dxfg_observable_list_model_listener_t *
dxfg_ObservableListModelListener_new(graal_isolatethread_t *thread,
                                     dxfg_observable_list_model_listener_function user_func, void *user_data) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_ObservableListModel_addListener(graal_isolatethread_t *thread, dxfg_observable_list_model_t *list,
                                             dxfg_observable_list_model_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_ObservableListModel_removeListener(graal_isolatethread_t *thread, dxfg_observable_list_model_t *list,
                                                dxfg_observable_list_model_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_order_book_model_listener_t *dxfg_OrderBookModelListener_new(graal_isolatethread_t *thread,
                                                                  dxfg_order_book_model_listener_function user_func,
                                                                  void *user_data) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_OrderBookModel_addListener(graal_isolatethread_t *thread, dxfg_order_book_model_t *model,
                                        dxfg_order_book_model_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_OrderBookModel_removeListener(graal_isolatethread_t *thread, dxfg_order_book_model_t *model,
                                           dxfg_order_book_model_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_event_type_list *dxfg_ObservableListModel_toArray(graal_isolatethread_t *thread,
                                                       dxfg_observable_list_model_t *list) {
    return INVALID_PTR_RESULT_NULLPTR;
}
