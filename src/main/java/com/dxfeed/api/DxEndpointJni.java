package com.dxfeed.api;

import com.dxfeed.api.osub.WildcardSymbol;
import com.dxfeed.event.EventType;

import java.beans.PropertyChangeListener;
import java.util.Iterator;
import java.util.Set;

public class DxEndpointJni {
  // callbacks from native
  private static DXEndpoint getInstance(int dxfgEndpointRole) {
    DXEndpoint.Role role = DXEndpoint.Role.values()[dxfgEndpointRole];
    return DXEndpoint.getInstance(role);
  }

  private static DXEndpoint.Builder buildWithRole(DXEndpoint.Builder builder, int dxfgEndpointRole) {
    DXEndpoint.Role role = DXEndpoint.Role.values()[dxfgEndpointRole];
    return builder.withRole(role);
  }

  private static int getRole(DXEndpoint endpoint) {
    return endpoint.getRole().ordinal();
  }

  private static int getState(DXEndpoint endpoint) {
    return endpoint.getState().ordinal();
  }

  private static long newStateChangeEventListener(long userCallback, long userData) {
    long id = DxFeedJni.nextHandleId();
    PropertyChangeListener listener = event -> nOnStateChangeListener(
        ((DXEndpoint.State) event.getOldValue()).ordinal(),
        ((DXEndpoint.State) event.getNewValue()).ordinal(),
        userCallback, userData);
    DxFeedJni.nativeObjectsMap.put(id, listener);
    return id;
  }

  private static void addStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
    PropertyChangeListener propertyChangeListener =
        (PropertyChangeListener) DxFeedJni.nativeObjectsMap.get(nativeHandleId);
    if (propertyChangeListener != null) {
      endpoint.addStateChangeListener(propertyChangeListener);
    }
  }

  private static void removeStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
    PropertyChangeListener stateChangeListener =
        (PropertyChangeListener) DxFeedJni.nativeObjectsMap.remove(nativeHandleId);
    if (stateChangeListener != null) {
      endpoint.removeStateChangeListener(stateChangeListener);
    }
  }

  private static byte[] getEventTypes(DXEndpoint endpoint) {
    Set<Class<? extends EventType<?>>> eventTypes = endpoint.getEventTypes();
    Iterator<Class<? extends EventType<?>>> iterator = eventTypes.iterator();
    int eventsCount = eventTypes.size();
    byte[] pEventTypes = new byte[eventsCount];
    for (int i = 0; i < eventsCount && iterator.hasNext(); ++i) {
      pEventTypes[i] = DxfgEventClazzT.fromClass(iterator.next());
    }
    return pEventTypes;
  }

  // todo: stopUserCallbackProcessorThread when Subscription event listener is removed
  private static void awaitProcessed(DXEndpoint endpoint) throws InterruptedException {
    endpoint.awaitProcessed();
    nStopUserCallbackProcessorThread();
  }

  private static native void nOnStateChangeListener(int oldState, int newState, long userCallback, long userData);
  private static native void nStopUserCallbackProcessorThread();

  public static void main(String[] args) throws InterruptedException {
    tapeFile();
  }

  public static void tapeFile() throws InterruptedException {
    String inputAddress = "file:ConvertTapeFile.in[readAs=stream_data,speed=max]";
    String outputAddress = "tape:ConvertTapeFile.out[saveAs=stream_data,format=text]";

    // Create input endpoint configured for tape reading
    DXEndpoint inputEndpoint = DXEndpoint.newBuilder()
        .withRole(DXEndpoint.Role.STREAM_FEED) // prevents event conflation and loss due to buffer overflow
        .withProperty(DXEndpoint.DXFEED_WILDCARD_ENABLE_PROPERTY, "true") // enables wildcard subscription
        .withProperty(DXEndpoint.DXENDPOINT_EVENT_TIME_PROPERTY, "true") // use provided event times
        .build();
    // Create output endpoint configured for tape writing
    DXEndpoint outputEndpoint = DXEndpoint.newBuilder()
        .withRole(DXEndpoint.Role.STREAM_PUBLISHER) // prevents event conflation and loss due to buffer overflow
        .withProperty(DXEndpoint.DXFEED_WILDCARD_ENABLE_PROPERTY, "true") // enables wildcard subscription
        .withProperty(DXEndpoint.DXENDPOINT_EVENT_TIME_PROPERTY, "true") // use provided event times
        .build();

    // Create and link event processor for all types of events
    // Note: set of processed event types could be limited if needed
    Class<? extends EventType<?>>[] eventTypes = inputEndpoint.getEventTypes().toArray(new Class[inputEndpoint.getEventTypes().size()]);
    DXFeedSubscription<? extends EventType<?>> sub = inputEndpoint.getFeed().createSubscription(eventTypes);
    sub.addEventListener(events -> {
      // Here event processing occurs. Events could be modified, removed, or new events added.
        /* For example, the below code adds 1 hour to event times:
        for (EventType event : events)
            event.setEventTime(event.getEventTime() + 3600_000);
        */
      // Publish processed events
      outputEndpoint.getPublisher().publishEvents(events);
    });
    // Subscribe to all symbols
    // Note: set of processed symbols could be limited if needed
    sub.addSymbols(WildcardSymbol.ALL);

    // Connect output endpoint and start output tape writing BEFORE starting input tape reading
    outputEndpoint.connect(outputAddress);
    // Connect input endpoint and start input tape reading AFTER starting output tape writing
    inputEndpoint.connect(inputAddress);

    // Wait until all data is read and processed, and then gracefully close input endpoint
    inputEndpoint.awaitNotConnected();
    inputEndpoint.closeAndAwaitTermination();

    // Wait until all data is processed and written, and then gracefully close output endpoint
    outputEndpoint.awaitProcessed();
    outputEndpoint.closeAndAwaitTermination();

  }

}
