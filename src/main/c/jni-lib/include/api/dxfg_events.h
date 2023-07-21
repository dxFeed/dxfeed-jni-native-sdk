// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_API_EVENTS_H_
#define DXFEED_JNI_NATIVE_API_EVENTS_H_

#ifdef __cplusplus
extern "C" {
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "dxfg_javac.h"

typedef enum dxfg_indexed_event_source_type_t {
  INDEXED_EVENT_SOURCE = 0,
  ORDER_SOURCE
} dxfg_indexed_event_source_type_t;

typedef struct dxfg_indexed_event_source_t {
  dxfg_indexed_event_source_type_t type;
  int32_t id;
  const char* name;
} dxfg_indexed_event_source_t;

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
  const char* symbol;
} dxfg_string_symbol_t;

typedef struct dxfg_wildcard_symbol_t {
  dxfg_symbol_t supper;
} dxfg_wildcard_symbol_t;

typedef struct dxfg_candle_symbol_t {
  dxfg_symbol_t supper;
  const char* symbol;
} dxfg_candle_symbol_t;

typedef struct dxfg_indexed_event_subscription_symbol_t {
  dxfg_symbol_t supper;
  dxfg_symbol_t* symbol;
  dxfg_indexed_event_source_t* source;
} dxfg_indexed_event_subscription_symbol_t;

typedef struct dxfg_time_series_subscription_symbol_t {
  dxfg_symbol_t supper;
  dxfg_symbol_t* symbol;
  int64_t from_time;
} dxfg_time_series_subscription_symbol_t;

typedef struct dxfg_symbol_list {
  int32_t size;
  dxfg_symbol_t** elements;
} dxfg_symbol_list;

/**
 * In the Java implementation, all events are lined up in a hierarchy
 * from the underlying EventType interface. In the C implementation,
 * we specify the type in the event in a separate field because we
 * cannot get it from the class name.
 */
typedef enum dxfg_event_clazz_t {
  DXFG_EVENT_QUOTE = 0,      // LASTING
  DXFG_EVENT_PROFILE,        // LASTING
  DXFG_EVENT_SUMMARY,        // LASTING
  DXFG_EVENT_GREEKS,         // LASTING + INDEXED -> TIME_SERIES
  DXFG_EVENT_CANDLE,         // LASTING + INDEXED -> TIME_SERIES
  DXFG_EVENT_DAILY_CANDLE,   // LASTING + INDEXED -> TIME_SERIES -> CANDLE
  DXFG_EVENT_UNDERLYING,     // LASTING + INDEXED -> TIME_SERIES
  DXFG_EVENT_THEO_PRICE,     // LASTING + INDEXED -> TIME_SERIES
  // abstract DXFG_EVENT_TRADE_BASE,     // LASTING
  DXFG_EVENT_TRADE,          // LASTING -> TRADE_BASE
  DXFG_EVENT_TRADE_ETH,      // LASTING -> TRADE_BASE
  DXFG_EVENT_CONFIGURATION,  // LASTING
  DXFG_EVENT_MESSAGE,        //
  DXFG_EVENT_TIME_AND_SALE,  // INDEXED -> TIME_SERIES
  DXFG_EVENT_ORDER_BASE,     // INDEXED
  DXFG_EVENT_ORDER,          // INDEXED -> ORDER_BASE
  DXFG_EVENT_ANALYTIC_ORDER, // INDEXED -> ORDER_BASE -> ORDER
  DXFG_EVENT_SPREAD_ORDER,   // INDEXED -> ORDER_BASE
  DXFG_EVENT_SERIES,         // INDEXED
  DXFG_EVENT_OPTION_SALE,    // INDEXED
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
  const char* event_symbol;
  int64_t event_time;
} dxfg_market_event_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Quote.html">Javadoc</a>
 */
typedef struct dxfg_quote_t {
  dxfg_market_event_t market_event;
  int32_t time_millis_sequence;
  int32_t time_nano_part;
  int64_t bid_time;
  int16_t bid_exchange_code;
  double bid_price;
  double bid_size;
  int64_t ask_time;
  int16_t ask_exchange_code;
  double ask_price;
  double ask_size;
} dxfg_quote_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Profile.html">Javadoc</a>
 */
typedef struct dxfg_profile_t {
  dxfg_market_event_t market_event;
  //    dxfg_lasting_event_t lasting_event;
  const char *description;
  const char *status_reason;
  int64_t halt_start_time;
  int64_t halt_end_time;
  double high_limit_price;
  double low_limit_price;
  double high_52_week_price;
  double low_52_week_price;
  double beta;
  double earnings_per_share;
  double dividend_frequency;
  double ex_dividend_amount;
  int32_t ex_dividend_day_id;
  double shares;
  double free_float;
  int32_t flags;
} dxfg_profile_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/Candle.html">Javadoc</a>
 */
typedef struct dxfg_candle_t {
  dxfg_event_type_t event_type;
  //    dxfg_time_series_event_t time_series_event;
  //    dxfg_lasting_event_t lasting_event;
  const char* event_symbol;
  int64_t event_time;
  int32_t event_flags;
  int64_t index;
  int64_t count;
  double open;
  double high;
  double low;
  double close;
  double volume;
  double vwap;
  double bid_volume;
  double ask_volume;
  double imp_volatility;
  double open_interest;
} dxfg_candle_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/candle/DailyCandle.html">Javadoc</a>
 */
typedef struct dxfg_daily_candle_t {
    dxfg_candle_t candle;
} dxfg_daily_candle_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Underlying.html">Javadoc</a>
 */
typedef struct dxfg_underlying_t {
  dxfg_market_event_t market_event;
  //    dxfg_time_series_event_t time_series_event;
  //    dxfg_lasting_event_t lasting_event;
  int32_t event_flags;
  int64_t index;
  double volatility;
  double front_volatility;
  double back_volatility;
  double call_volume;
  double put_volume;
  double put_call_ratio;
} dxfg_underlying_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TradeBase.html">Javadoc</a>
 */
typedef struct dxfg_trade_base_t {
  dxfg_market_event_t market_event;
  //    dxfg_lasting_event_t lasting_event;
  int64_t time_sequence;
  int32_t time_nano_part;
  int16_t exchange_code;
  double price;
  double change;
  double size;
  int32_t day_id;
  double day_volume;
  double day_turnover;
  int32_t flags;
} dxfg_trade_base_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Trade.html">Javadoc</a>
 */
typedef struct dxfg_trade_t {
  dxfg_trade_base_t trade_base;
} dxfg_trade_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/TimeAndSale.html">Javadoc</a>
 */
typedef struct dxfg_time_and_sale_t {
  dxfg_market_event_t market_event;
  int32_t event_flags;
  int64_t index;
  int32_t time_nano_part;
  int16_t exchange_code;
  double price;
  double size;
  double bid_price;
  double ask_price;
  const char* exchange_sale_conditions;
  int32_t flags;
  const char* buyer;
  const char* seller;
} dxfg_time_and_sale_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/OrderBase.html">Javadoc</a>
 */
typedef struct dxfg_order_base_t {
  dxfg_market_event_t market_event;
  //    dxfg_indexed_event_t indexed_event;
  int32_t event_flags;

  int64_t index;
  int64_t time_sequence;
  int32_t time_nano_part;

  int64_t action_time;
  int64_t order_id;
  int64_t aux_order_id;

  double price;
  double size;
  double executed_size;
  int64_t count;
  int32_t flags;

  int64_t trade_id;
  double trade_price;
  double trade_size;
} dxfg_order_base_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/Order.html">Javadoc</a>
 */
typedef struct dxfg_order_t {
  dxfg_order_base_t order_base;
  const char* market_maker;
} dxfg_order_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/AnalyticOrder.html">Javadoc</a>
 */
typedef struct dxfg_analytic_order_t {
  dxfg_order_t order;
  double iceberg_peak_size;
  double iceberg_hidden_size;
  double iceberg_executed_size;
  int32_t iceberg_flags;
} dxfg_analytic_order_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/market/SpreadOrder.html">Javadoc</a>
 */
typedef struct dxfg_spread_order_t {
  dxfg_order_base_t order_base;
  const char* spread_symbol;
} dxfg_spread_order_t;

/**
 * <a href="https://docs.dxfeed.com/dxfeed/api/com/dxfeed/event/option/Series.html">Javadoc</a>
 */
typedef struct dxfg_series_t {
  dxfg_market_event_t market_event;
  //    dxfg_indexed_event_t indexed_event;
  int32_t event_flags;
  int64_t index;
  int64_t time_sequence;
  int32_t expiration;
  double volatility;
  double call_volume;
  double put_volume;
  double put_call_ratio;
  double forward_price;
  double dividend;
  double interest;
} dxfg_series_t;

typedef struct dxfg_event_type_list {
  int32_t size;
  dxfg_event_type_t const* const* elements;
} dxfg_event_type_list;

typedef struct dxfg_event_clazz_list_t {
  int32_t size;
  dxfg_event_clazz_t** elements;
} dxfg_event_clazz_list_t;

dxfg_symbol_t*                dxfg_Symbol_new(graal_isolatethread_t* thread, const char* symbol, dxfg_symbol_type_t symbolType);
int32_t                       dxfg_Symbol_release(graal_isolatethread_t* thread, dxfg_symbol_t* symbol);
dxfg_event_type_t*            dxfg_EventType_new(graal_isolatethread_t* thread, const char* symbolName, dxfg_event_clazz_t clazz);
int32_t                       dxfg_EventType_release(graal_isolatethread_t* thread, dxfg_event_type_t* eventType);

// free the memory occupied by the с data structure (list and all events)
int32_t                       dxfg_CList_EventType_release(graal_isolatethread_t* thread,
                                                           dxfg_event_type_list* eventTypes);

// free the memory occupied by the с data structure (list and all int-pointer)
int32_t                       dxfg_CList_EventClazz_release(graal_isolatethread_t* thread,
                                                            dxfg_event_clazz_list_t* eventClazzes);

// free the memory occupied by the с data structure (list and all int-pointer)
int32_t                       dxfg_CList_symbol_release(graal_isolatethread_t* thread, dxfg_symbol_list*);

dxfg_indexed_event_source_t*  dxfg_IndexedEvent_getSource(graal_isolatethread_t* thread, dxfg_event_type_t* eventType);

// if source == nullptr, then return IndexedEventSource.DEFAULT else OrderSource
dxfg_indexed_event_source_t*  dxfg_IndexedEventSource_new(graal_isolatethread_t* thread, const char* source);
int32_t                       dxfg_IndexedEventSource_release(graal_isolatethread_t* thread, dxfg_indexed_event_source_t* source);

#ifdef __cplusplus
}
#endif

#endif // DXFEED_JNI_NATIVE_API_EVENTS_H_
