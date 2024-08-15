// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#include "api/dxfg_publisher.h"
#include "dxfeed/DxPublisher.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

dxfg_publisher_t *dxfg_DXPublisher_getInstance(graal_isolatethread_t *thread) {
    return dxfeed::DxPublisher::getInstance(thread);
}

int32_t dxfg_DXPublisher_publishEvents(graal_isolatethread_t *thread, dxfg_publisher_t *publisher,
                                       dxfg_event_type_list *events) {
    auto *pDxPublisher = dxfeed::r_cast<dxfeed::DxPublisher *>(publisher);
    return pDxPublisher->publishEvents(thread, events);
}

dxfg_observable_subscription_t *dxfg_DXPublisher_getSubscription(graal_isolatethread_t *thread,
                                                                 dxfg_publisher_t *publisher,
                                                                 dxfg_event_clazz_t eventClazz) {
    auto *pDxFeed = dxfeed::r_cast<dxfeed::DxPublisher *>(publisher);
    return dxfeed::r_cast<dxfg_observable_subscription_t *>(pDxFeed->getSubscription(thread, eventClazz));
}

int32_t dxfg_ObservableSubscription_isClosed(graal_isolatethread_t *thread, dxfg_observable_subscription_t *sub) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

dxfg_event_clazz_list_t *dxfg_ObservableSubscription_getEventTypes(graal_isolatethread_t *thread,
                                                                   dxfg_observable_subscription_t *sub) {
    return INVALID_PTR_RESULT_NULLPTR;
}

int32_t dxfg_ObservableSubscription_containsEventType(graal_isolatethread_t *thread,
                                                      dxfg_observable_subscription_t *sub,
                                                      dxfg_event_clazz_t eventClazz) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_ObservableSubscription_addChangeListener(graal_isolatethread_t *thread,
                                                      dxfg_observable_subscription_t *sub,
                                                      dxfg_observable_subscription_change_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}

int32_t dxfg_ObservableSubscription_removeChangeListener(graal_isolatethread_t *thread,
                                                         dxfg_observable_subscription_t *sub,
                                                         dxfg_observable_subscription_change_listener_t *listener) {
    return INVALID_INT_RESULT_MINUS_ONE;
}
