// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "api/dxfg_api.h"

class Deleter {
  graal_isolatethread_t* thread;
public:
  explicit Deleter(graal_isolatethread_t* thread) : thread(thread) {}

  void operator()(dxfg_event_type_list* p) const {
    std::cout << "Deleter is deleting a dxfg_event_type_list" << std::endl;
    dxfg_CList_EventType_release(thread, p);
  };
};

/**
 * Write events to a tape file.
 *
 * */
int main() {
  // Create thread on which all the work will be executed.
  auto thread = create_thread();
  if (thread == nullptr) {
    return -1;
  }

  auto endpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  // Is required for tape connector to be able to receive everything.
  dxfg_DXEndpoint_Builder_withProperty(thread, endpointBuilder, "dxfeed.wildcard.enable", "true");
  dxfg_DXEndpoint_Builder_withRole(thread, endpointBuilder, DXFG_ENDPOINT_ROLE_PUBLISHER);
  dxfg_endpoint_t* pEndpoint = dxfg_DXEndpoint_Builder_build(thread, endpointBuilder);

  // Connect to the address, remove [format=text] or change on [format=binary] for binary format
  dxfg_DXEndpoint_connect(thread, pEndpoint, "tape:WriteTapeFile.out.txt[format=text]");
  
  // Get publisher.
  dxfg_publisher_t* pPublisher = dxfg_DXEndpoint_getPublisher(thread, pEndpoint);

  // Creates new Quote market events.
  auto quote1 = new dxfg_quote_t();
  quote1->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
  quote1->market_event.event_symbol = "TEST1";
  quote1->bid_price = 10.1;
  quote1->ask_price = 10.2;

  auto quote2 = new dxfg_quote_t();
  quote2->market_event.event_type.clazz = DXFG_EVENT_QUOTE;
  quote2->market_event.event_symbol = "TEST2";
  quote2->bid_price = 17.1;
  quote2->ask_price = 17.2;

  Deleter deleter(thread);
  auto list = std::unique_ptr<dxfg_event_type_list, Deleter>(new dxfg_event_type_list(), deleter);
  list->size = 2;
  list->elements = new dxfg_event_type_t*[2];
  list->elements[0] = reinterpret_cast<dxfg_event_type_t*>(quote1);
  list->elements[1] = reinterpret_cast<dxfg_event_type_t*>(quote2);

  // Publish events.
  dxfg_DXPublisher_publishEvents(thread, pPublisher, list.get());

  // Wait until all data is written, close, and wait until it closes.
  dxfg_DXEndpoint_awaitProcessed(thread, pEndpoint);
  dxfg_DXEndpoint_closeAndAwaitTermination(thread, pEndpoint);
}
