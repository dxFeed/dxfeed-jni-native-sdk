// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_subscription.h"
#include "dxfeed/DxSubscription.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_subscription_t* dxfg_DXFeedSubscription_new(graal_isolatethread_t* thread, dxfg_event_clazz_t eventClass) {
  return dxfeed::r_cast<dxfg_subscription_t*>(new dxfeed::DxSubscription(thread, eventClass));
}

dxfg_subscription_t* dxfg_DXFeedSubscription_new2(graal_isolatethread_t* thread,
                                                  dxfg_event_clazz_list_t* eventClasses)
{
  return dxfeed::r_cast<dxfg_subscription_t*>(new dxfeed::DxSubscription(thread, eventClasses));
}

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

int32_t dxfg_DXFeedSubscription_removeSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                             dxfg_symbol_t* symbol)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->removeSymbol(thread, symbol);
}

int32_t dxfg_DXFeedSubscription_removeSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                              dxfg_symbol_list* symbols)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->removeSymbols(thread, symbols);
}

int32_t dxfg_DXFeedSubscription_clear(graal_isolatethread_t *thread, dxfg_subscription_t *sub) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->clear(thread);
}

int32_t dxfg_DXFeedSubscription_attach(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_feed_t* feed) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return pDxSubscription->attach(thread, pDxFeed);
}

int32_t dxfg_DXFeedSubscription_detach(graal_isolatethread_t* thread, dxfg_subscription_t* sub, dxfg_feed_t* feed) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return pDxSubscription->detach(thread, pDxFeed);
}

int32_t dxfg_DXFeedSubscription_isClosed(graal_isolatethread_t* thread, dxfg_subscription_t* sub) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->isClosed(thread);
}

dxfg_event_clazz_list_t* dxfg_DXFeedSubscription_getEventTypes(graal_isolatethread_t* thread, dxfg_subscription_t* sub)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->getEventTypes(thread);
}

int32_t dxfg_DXFeedSubscription_containsEventType(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                                  dxfg_event_clazz_t eventClazz)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->containsEventType(thread, eventClazz);
}

dxfg_symbol_list* dxfg_DXFeedSubscription_getSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->getSymbols(thread);
}


int32_t dxfg_DXFeedSubscription_setSymbol(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                          dxfg_symbol_t* pSymbol)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->setSymbol(thread, pSymbol);
}

int32_t dxfg_DXFeedSubscription_setSymbols(graal_isolatethread_t* thread, dxfg_subscription_t* sub,
                                           dxfg_symbol_list* symbols)
{
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxSubscription*>(sub);
  return pDxSubscription->setSymbols(thread, symbols);
}

int32_t dxfg_DXFeedTimeSeriesSubscription_setFromTime(graal_isolatethread_t* thread,
                                                      dxfg_time_series_subscription_t* sub, int64_t fromTime) {
  auto* pDxSubscription = dxfeed::r_cast<dxfeed::DxTimeSeriesSubscription*>(sub);
  return pDxSubscription->setTime(thread, fromTime);
}