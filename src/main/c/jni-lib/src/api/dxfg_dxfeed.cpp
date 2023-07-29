// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_feed.h"
#include "dxfeed/DxFeed.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_feed_t* dxfg_DXFeed_getInstance(graal_isolatethread_t* thread) {
  return dxfeed::DxFeed::getInstance(thread);
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription(graal_isolatethread_t* thread,
                                                    dxfg_feed_t* feed, dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return dxfeed::r_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(thread, eventClazz));
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription2(graal_isolatethread_t* thread,
                                                     dxfg_feed_t* feed, dxfg_event_clazz_list_t* eventClazzes)
{
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return dxfeed::r_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(thread, eventClazzes));
}

dxfg_time_series_subscription_t* dxfg_DXFeed_createTimeSeriesSubscription(graal_isolatethread_t* thread,
                                                                           dxfg_feed_t* feed,
                                                                           dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return dxfeed::r_cast<dxfg_time_series_subscription_t*>(pDxFeed->createTimeSeriesSubscription(thread, eventClazz));
}

dxfg_time_series_subscription_t*  dxfg_DXFeed_createTimeSeriesSubscription2(graal_isolatethread_t* thread,
                                                                            dxfg_feed_t* feed,
                                                                            dxfg_event_clazz_list_t* eventClazzes)
{
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return dxfeed::r_cast<dxfg_time_series_subscription_t*>(pDxFeed->createTimeSeriesSubscription(thread, eventClazzes));
}

int32_t dxfg_DXFeed_attachSubscription(graal_isolatethread_t* thread, dxfg_feed_t* feed, dxfg_subscription_t* sub) {
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  pDxFeed->attachSubscription(thread, sub);
  return JNI_OK;
}

int32_t dxfg_DXFeed_detachSubscription(graal_isolatethread_t* thread, dxfg_feed_t* feed, dxfg_subscription_t* sub) {
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  pDxFeed->detachSubscription(thread, sub);
  return JNI_OK;
}

int32_t dxfg_DXFeed_detachSubscriptionAndClear(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                               dxfg_subscription_t* sub)
{
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  pDxFeed->detachSubscriptionAndClear(thread, sub);
  return JNI_OK;
}

dxfg_event_type_t* dxfg_DXFeed_getLastEventIfSubscribed(graal_isolatethread_t* thread, dxfg_feed_t* feed,
                                                        dxfg_event_clazz_t eventClazz, dxfg_symbol_t* symbol)
{
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  return pDxFeed->getLastEventIfSubscribed(thread, eventClazz, symbol);
}

int32_t dxfg_DXFeed_getLastEvent(graal_isolatethread_t* thread, dxfg_feed_t* feed, dxfg_event_type_t* event) {
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  pDxFeed->getLastEvent(thread, event);
  return JNI_OK;
}

int32_t dxfg_DXFeed_getLastEvents(graal_isolatethread_t* thread, dxfg_feed_t* feed, dxfg_event_type_list* events) {
  auto* pDxFeed = dxfeed::r_cast<dxfeed::DxFeed*>(feed);
  pDxFeed->getLastEvents(thread, events);
  return JNI_OK;
}