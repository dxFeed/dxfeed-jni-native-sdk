// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_subscription.h"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

int32_t dxfg_DXSubscription_release(graal_isolatethread_t*, dxfg_subscription_t* subscription) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(subscription);
  delete pDxSubscription;
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_close(graal_isolatethread_t* thread, dxfg_subscription_t* sub) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  pDxSubscription->close(thread);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_addEventListener(graal_isolatethread_t* thread,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxEventListener =  dxfeed::r_cast<dxfeed::DxEventListener*>(listener);
  pDxSubscription->addListener(thread, pDxEventListener);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_removeEventListener(graal_isolatethread_t* thread,
                                                 dxfg_subscription_t* sub, dxfg_feed_event_listener_t* listener)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxEventListener =  dxfeed::r_cast<dxfeed::DxEventListener*>(listener);
  pDxSubscription->removeListener(thread, pDxEventListener);
  return JNI_OK;
}

int32_t dxfg_DXFeedSubscription_addSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* pSymbol)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->addSymbol(thread, pSymbol);
}

int32_t dxfg_DXFeedSubscription_addSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                           dxfg_symbol_list* symbols)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->addSymbols(thread, symbols);
}

int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* pSymbol)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->setSymbol(thread, pSymbol);
}