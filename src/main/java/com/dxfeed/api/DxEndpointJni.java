package com.dxfeed.api;

import java.beans.PropertyChangeListener;
import java.util.concurrent.ConcurrentHashMap;

public class DxEndpointJni {
  // callbacks from native
  private static DXEndpoint getInstance(int dxfgEndpointRole) {
    DXEndpoint.Role role = DXEndpoint.Role.values()[dxfgEndpointRole];
    return DXEndpoint.getInstance(role);
  }

  private static int getRole(DXEndpoint endpoint) {
    return endpoint.getRole().ordinal();
  }

  private static int getState(DXEndpoint endpoint) {
    return endpoint.getState().ordinal();
  }

  private static long newStateChangeEventListener(long userCallback, long userData) {
    long id = DxFeedJni.nextHandleId();
    System.out.println("DxEndpointJni::newStateChangeEventListener, nativeHandle = " + id);
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
      System.out.println("DxEndpointJni::addStateChangeEventListener, nativeHandle = " + nativeHandleId);
      endpoint.addStateChangeListener(propertyChangeListener);
    }
  }

  private static void removeStateChangeEventListener(DXEndpoint endpoint, long nativeHandleId) {
    PropertyChangeListener stateChangeListener =
        (PropertyChangeListener) DxFeedJni.nativeObjectsMap.remove(nativeHandleId);
    if (stateChangeListener != null) {
      System.out.println("DxEndpointJni::removeStateChangeEventListener, nativeHandle = " + nativeHandleId);
      endpoint.removeStateChangeListener(stateChangeListener);
    }
  }

  private static native void nOnStateChangeListener(int oldState, int newState, long userCallback, long userData);
}
