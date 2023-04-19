// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_GRAAL_NATIVE_API_EVENTS_H_
#define DXFEED_GRAAL_NATIVE_API_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#    include <cstdint>
#else
#    include <stdint.h>
#endif

#include "dxfg_javac.h"

typedef enum dxfg_symbol_type_t {
    STRING = 0,
    CANDLE,
    WILDCARD,
    INDEXED_EVENT_SUBSCRIPTION,
    TIME_SERIES_SUBSCRIPTION
} dxfg_symbol_type_t;

typedef struct dxfg_symbol_t {
    dxfg_symbol_type_t type;
} dxfg_symbol_t;

typedef struct dxfg_string_symbol_t {
    dxfg_symbol_t supper;
    const char *symbol;
} dxfg_string_symbol_t;

typedef struct dxfg_time_series_subscription_symbol_t {
    dxfg_symbol_t supper;
    dxfg_symbol_t *symbol;
    int64_t from_time;
} dxfg_time_series_subscription_symbol_t;

typedef struct dxfg_symbol_list {
    int32_t size;
    dxfg_symbol_t **elements;
} dxfg_symbol_list;

/**
 * In the Java implementation, all events are lined up in a hierarchy
 * from the underlying EventType interface. In the C implementation,
 * we specify the type in the event in a separate field because we
 * cannot get it from the class name.
 */
typedef enum dxfg_event_clazz_t {
    DXFG_EVENT_QUOTE = 0,      // LASTING
    DXFG_EVENT_TIME_AND_SALE,  // INDEXED -> TIME_SERIES
} dxfg_event_clazz_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/EventType.html">Javadoc</a>
 */
typedef struct dxfg_event_type_t {
    dxfg_event_clazz_t clazz;
} dxfg_event_type_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/MarketEvent.html">Javadoc</a>
 */
typedef struct dxfg_market_event_t {
    dxfg_event_type_t event_type;
    const char *event_symbol;
    int64_t event_time;
} dxfg_market_event_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TimeAndSale.html">Javadoc</a>
 */
typedef struct dxfg_time_and_sale_t {
    dxfg_market_event_t market_event;
    //    dxfg_time_series_event_t time_series_event;
    int32_t event_flags;
    int64_t index;
    int32_t time_nano_part;
    int16_t exchange_code;
    double price;
    double size;
    double bid_price;
    double ask_price;
    const char *exchange_sale_conditions;
    int32_t flags;
    const char *buyer;
    const char *seller;
} dxfg_time_and_sale_t;


typedef struct dxfg_event_type_list {
    int32_t size;
    dxfg_event_type_t **elements;
} dxfg_event_type_list;

typedef struct dxfg_event_clazz_list_t {
    int32_t size;
    dxfg_event_clazz_t **elements;
} dxfg_event_clazz_list_t;

dxfg_symbol_t*              dxfg_Symbol_new(graal_isolatethread_t *thread, const char *symbol, dxfg_symbol_type_t symbolType);
int32_t                     dxfg_Symbol_release(graal_isolatethread_t *thread, dxfg_symbol_t* symbol);
dxfg_event_type_t*          dxfg_EventType_new(graal_isolatethread_t *thread, const char *symbolName, dxfg_event_clazz_t clazz);
int32_t                     dxfg_EventType_release(graal_isolatethread_t *thread, dxfg_event_type_t* eventType);
int32_t                     dxfg_CList_EventType_release(graal_isolatethread_t *thread, dxfg_event_type_list* eventTypes);// free the memory occupied by the с data structure (list and all events)
int32_t                     dxfg_CList_EventClazz_release(graal_isolatethread_t *thread, dxfg_event_clazz_list_t* eventClazzes);// free the memory occupied by the с data structure (list and all int-pointer)
int32_t                     dxfg_CList_symbol_release(graal_isolatethread_t *thread, dxfg_symbol_list*);// free the memory occupied by the с data structure (list and all int-pointer)

#ifdef __cplusplus
}
#endif

#endif // DXFEED_GRAAL_NATIVE_API_EVENTS_H_
