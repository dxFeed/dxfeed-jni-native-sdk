// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_feed.h"
#include "dxfeed/DxFeed.hpp"

dxfg_subscription_t* dxfg_DXFeed_createSubscription(graal_isolatethread_t* thread,
                                                    dxfg_feed_t* feed, dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(thread, eventClazz));
}

dxfg_subscription_t* dxfg_DXFeed_createSubscription2(graal_isolatethread_t* thread,
                                                     dxfg_feed_t* feed, dxfg_event_clazz_list_t* eventClazzes)
{
  auto* pDxFeed = reinterpret_cast<dxfeed::DxFeed*>(feed);
  return reinterpret_cast<dxfg_subscription_t*>(pDxFeed->createSubscription(thread, eventClazzes));
}