// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/dxfg_publisher.h"
#include "dxfeed/DxPublisher.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_publisher_t* dxfg_DXPublisher_getInstance(graal_isolatethread_t* thread) {
  return dxfeed::DxPublisher::getInstance(thread);
}

int32_t dxfg_DXPublisher_publishEvents(graal_isolatethread_t* thread, dxfg_publisher_t* publisher,
                                       dxfg_event_type_list* events)
{
  auto* pDxPublisher = dxfeed::r_cast <dxfeed::DxPublisher*>(publisher);
  return pDxPublisher->publishEvents(thread, events);
}

dxfg_observable_subscription_t* dxfg_DXPublisher_getSubscription(graal_isolatethread_t* thread,
                                                                 dxfg_publisher_t* publisher,
                                                                 dxfg_event_clazz_t eventClazz)
{
  auto* pDxFeed = dxfeed::r_cast <dxfeed::DxPublisher*>(publisher);
  return dxfeed::r_cast<dxfg_observable_subscription_t*>(pDxFeed->getSubscription(thread, eventClazz));
}
