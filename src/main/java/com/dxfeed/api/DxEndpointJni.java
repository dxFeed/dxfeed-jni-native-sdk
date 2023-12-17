// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api;

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

  private static native void nOnStateChangeListener(int oldState, int newState, long userCallback, long userData);
}
