// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_subscription.h"
#include "dxfeed/DxSubscription.hpp"

int32_t dxfg_DXSubscription_release(graal_isolatethread_t*, dxfg_subscription_t* subscription) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(subscription);
  delete pDxSubscription;
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_close(graal_isolatethread_t* thread, dxfg_subscription_t* sub) {
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  pDxSubscription->close(thread);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t* thread,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxEventListener =  reinterpret_cast<dxfeed::DxEventListener*>(listener);
  pDxSubscription->addListener(thread, pDxEventListener);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_removeEventListener(graal_isolatethread_t* thread,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxEventListener =  reinterpret_cast<dxfeed::DxEventListener*>(listener);
  pDxSubscription->removeListener(thread, pDxEventListener);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* pSymbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (pSymbol->type) {
    case STRING: {
      auto* pStringSymbol = reinterpret_cast<dxfg_string_symbol_t*>(pSymbol);
      pDxSubscription->addSymbol(thread, pStringSymbol->supper.type, pStringSymbol->symbol);
      break;
    }
    case WILDCARD: {
      auto* pWildcardSymbol = reinterpret_cast<dxfg_wildcard_symbol_t*>(pSymbol);
      pDxSubscription->addSymbol(thread, pWildcardSymbol->supper.type, "*");
      break;
    }
    case CANDLE: {
      auto* pCandleSymbol = reinterpret_cast<dxfg_candle_symbol_t*>(pSymbol);
      pDxSubscription->addSymbol(thread, pCandleSymbol->supper.type, pCandleSymbol->symbol);
      break;
    }
    case TIME_SERIES_SUBSCRIPTION: {
//      auto* pSymbol = reinterpret_cast<dxfg_time_series_subscription_symbol_t*>(pSymbolType);
//      pDxSubscription->addSymbol(thread, pSymbol->);
      break;
    }
    case INDEXED_EVENT_SUBSCRIPTION: {
      break;
    }
    default:
      return JNI_ERR;
  }
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* pSymbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (pSymbol->type) {
    case STRING: {
      auto* pStringSymbol = reinterpret_cast<dxfg_string_symbol_t*>(pSymbol);
      pDxSubscription->setSymbol(thread, pStringSymbol->supper.type, pStringSymbol->symbol);
      break;
    }
    case WILDCARD: {
      auto* pWildcardSymbol = reinterpret_cast<dxfg_wildcard_symbol_t*>(pSymbol);
      pDxSubscription->setSymbol(thread, pWildcardSymbol->supper.type, "*");
      break;
    }
    case CANDLE: {
      auto* pCandleSymbol = reinterpret_cast<dxfg_candle_symbol_t*>(pSymbol);
      pDxSubscription->setSymbol(thread, pCandleSymbol->supper.type, pCandleSymbol->symbol);
      break;
    }
    case TIME_SERIES_SUBSCRIPTION: {
//      auto* pSymbol = reinterpret_cast<dxfg_time_series_subscription_symbol_t*>(pSymbolType);
//      pDxSubscription->addSymbol(thread, pSymbol->);
      break;
    }
    case INDEXED_EVENT_SUBSCRIPTION: {
      break;
    }
    default:
      return JNI_ERR;
  }
  return JNI_OK;
}