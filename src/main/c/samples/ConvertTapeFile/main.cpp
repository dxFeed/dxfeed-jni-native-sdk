// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <string>

#include "api/dxfg_endpoint.h"
#include "api/dxfg_feed.h"
#include "api/dxfg_subscription.h"
#include "api/dxfg_publisher.h"

void publishEvents(graal_isolatethread_t* thread, dxfg_event_type_list *events, void *user_data) {
  auto* outputEndpoint = reinterpret_cast<dxfg_endpoint_t*>(user_data);
  auto* pPublisher = dxfg_DXEndpoint_getPublisher(thread, outputEndpoint);
  dxfg_DXPublisher_publishEvents(thread, pPublisher, events);
}

/** Converts one tape file into another tape file with optional intermediate processing or filtering.
 *  This project contains sample tape file ConvertTapeFile.in.
 *
 *  todo: copy file "ConvertTapeFile.in" to build directory before start the sample
 */
int main(int argc, char** argv) {
  // Create thread on which all the work will be executed.
  auto thread = create_thread();
  if (thread == nullptr) {
    return -1;
  }

  // Determine input and output tapes and specify appropriate configuration parameters.
  std::string inputAddress = argc > 1 ? argv[1] : "file:ConvertTapeFile.in[readAs=stream_data,speed=max]";
  std::string outputAddress = argc > 2 ? argv[2] : "tape:ConvertTapeFile.out[saveAs=stream_data,format=text]";

  // Create input endpoint configured for tape reading.
  auto* inputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  dxfg_DXEndpoint_Builder_withRole(thread, inputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_FEED);
  dxfg_DXEndpoint_Builder_withProperty(thread, inputEndpointBuilder, "dxfeed.wildcard.enable", "true");
  dxfg_DXEndpoint_Builder_withProperty(thread, inputEndpointBuilder, "dxendpoint.eventTime", "true");
  auto inputEndpoint = dxfg_DXEndpoint_Builder_build(thread, inputEndpointBuilder);

  // Create output endpoint configured for tape writing.
  auto* outputEndpointBuilder = dxfg_DXEndpoint_newBuilder(thread);
  dxfg_DXEndpoint_Builder_withRole(thread, outputEndpointBuilder, DXFG_ENDPOINT_ROLE_STREAM_PUBLISHER);
  dxfg_DXEndpoint_Builder_withProperty(thread, outputEndpointBuilder, "dxfeed.wildcard.enable", "true");
  dxfg_DXEndpoint_Builder_withProperty(thread, outputEndpointBuilder, "dxendpoint.eventTime", "true");
  dxfg_endpoint_t* outputEndpoint = dxfg_DXEndpoint_Builder_build(thread, outputEndpointBuilder);

  // Create and link event processor for all types of events.
  // Note: Set of processed event types could be limited if needed.
  auto* pList = dxfg_DXEndpoint_getEventTypes(thread, inputEndpoint);
  auto* pFeed = dxfg_DXEndpoint_getFeed(thread, inputEndpoint);
  auto* pSubscription = dxfg_DXFeed_createSubscription2(thread, pFeed, pList);
  auto* listener = dxfg_DXFeedEventListener_new(thread, &publishEvents, outputEndpoint);
  dxfg_DXFeedSubscription_addEventListener(thread, pSubscription, listener);

  // Subscribe to all symbols.
  // Note: Set of processed symbols could be limited if needed.
  dxfg_wildcard_symbol_t symbolWildcard;
  symbolWildcard.supper.type = WILDCARD;
  dxfg_DXFeedSubscription_addSymbol(thread, pSubscription, reinterpret_cast<dxfg_symbol_t*>(&symbolWildcard));

  // Connect output endpoint and start output tape writing BEFORE starting input tape reading.
  dxfg_DXEndpoint_connect(thread, outputEndpoint, outputAddress.c_str());
  // Connect input endpoint and start input tape reading AFTER starting output tape writing.
  dxfg_DXEndpoint_connect(thread, inputEndpoint, inputAddress.c_str());

  // Wait until all data is read and processed, and then gracefully close input endpoint.
  dxfg_DXEndpoint_awaitNotConnected(thread, inputEndpoint);
  dxfg_DXEndpoint_closeAndAwaitTermination(thread, inputEndpoint);

  // Wait until all data is processed and written, and then gracefully close output endpoint.
  dxfg_DXEndpoint_awaitProcessed(thread, outputEndpoint);
  dxfg_DXEndpoint_closeAndAwaitTermination(thread, outputEndpoint);

  // Close subscription and clear resources.
  dxfg_DXFeedSubscription_close(thread, pSubscription);
  dxfg_JavaObjectHandler_release(thread, &listener->handler);
  dxfg_JavaObjectHandler_release(thread, &inputEndpointBuilder->handler);
  dxfg_JavaObjectHandler_release(thread, &inputEndpoint->handler);
  dxfg_JavaObjectHandler_release(thread, &outputEndpointBuilder->handler);
  dxfg_JavaObjectHandler_release(thread, &outputEndpoint->handler);
}