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

int32_t dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* symbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (symbol->type) {
    case STRING: {
      auto* pSymbol = reinterpret_cast<dxfg_string_symbol_t*>(symbol);
      // todo: investigate, why there no setSymbols method. Inlined?
      //  for now use addSymbol instead of setSymbol
      pDxSubscription->addSymbol(thread, pSymbol->symbol);
      break;
    }
    default:
      return JNI_ERR;
  }
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* symbol)
{
  auto* pDxSubscription = reinterpret_cast<dxfeed::DxSubscription*>(sub);
  switch (symbol->type) {
    case STRING: {
      auto* pSymbol = reinterpret_cast<dxfg_string_symbol_t*>(symbol);
      pDxSubscription->addSymbol(thread, pSymbol->symbol);
      break;
    }
    default:
      return JNI_ERR;
  }
  return JNI_OK;
}