// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api;

import com.dxfeed.api.osub.ObservableSubscriptionChangeListener;
import com.dxfeed.event.EventType;

import java.util.Iterator;
import java.util.Set;

public class DxSubscriptionJni {
  // callbacks from native
  private static long newEventListener(long userCallback, long userData) {
    long id = DxFeedJni.nextHandleId();
    DXFeedEventListener<EventType<?>> listener = eventList -> {
      NativeEventsList<EventType<?>> nativeTS = new NativeEventsList<>(eventList);
      nOnEventListener(eventList.size(), nativeTS.byteData(), nativeTS.doubleData(), nativeTS.pEventTypes,
          userCallback, userData);
      nativeTS.clear();
    };
    DxFeedJni.nativeObjectsMap.put(id, listener);
    return id;
  }

  private static void addEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
    DXFeedEventListener<EventType<?>> eventListener =
        (DXFeedEventListener<EventType<?>>) DxFeedJni.nativeObjectsMap.get(nativeHandleId);
    if (eventListener != null) {
      sub.addEventListener(eventListener);
    }
  }

  private static void removeEventListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
    DXFeedEventListener<EventType<?>> eventListener =
        (DXFeedEventListener<EventType<?>>) DxFeedJni.nativeObjectsMap.remove(nativeHandleId);
    if (eventListener != null) {
      sub.removeEventListener(eventListener);
    }
  }

  private static long newChangeListener(
      long symbolAddedCallback,
      long symbolRemovedCallback,
      long subscriptionRemovedCallback,
      long userData
  ) {
    long id = DxFeedJni.nextHandleId();
    ObservableSubscriptionChangeListener listener = new ObservableSubscriptionChangeListener() {
      @Override
      public void symbolsAdded(Set<?> symbols) {
        nOnSymbolsAdded(symbols.size(), symbols.toArray(), symbolAddedCallback, userData);
      }

      @Override
      public void symbolsRemoved(Set<?> symbols) {
        nOnSymbolsRemoved(symbols.size(), symbols.toArray(), symbolRemovedCallback, userData);
      }

      @Override
      public void subscriptionClosed() {
        nOnSubscriptionClosed(subscriptionRemovedCallback, userData);
      }
    };
    DxFeedJni.nativeObjectsMap.put(id, listener);
    return id;
  }

  private static void addChangeListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
    ObservableSubscriptionChangeListener stateChangeListener =
        (ObservableSubscriptionChangeListener) DxFeedJni.nativeObjectsMap.get(nativeHandleId);
    if (stateChangeListener != null) {
      sub.addChangeListener(stateChangeListener);
    }
  }

  private static void removeChangeListener(DXFeedSubscription<EventType<?>> sub, long nativeHandleId) {
    ObservableSubscriptionChangeListener stateChangeListener =
        (ObservableSubscriptionChangeListener) DxFeedJni.nativeObjectsMap.remove(nativeHandleId);
    if (stateChangeListener != null) {
      sub.removeChangeListener(stateChangeListener);
    }
  }

  private static byte[] getEventTypes(DXFeedSubscription<EventType<?>> sub) {
    Set<Class<? extends EventType<?>>> eventTypes = sub.getEventTypes();
    Iterator<Class<? extends EventType<?>>> iterator = eventTypes.iterator();
    int eventsCount = eventTypes.size();
    byte[] pEventTypes = new byte[eventsCount];
    for (int i = 0; i < eventsCount && iterator.hasNext(); ++i) {
      pEventTypes[i] = DxfgEventClazzT.fromClass(iterator.next());
    }
    return pEventTypes;
  }

  private static Object[] getSymbols(DXFeedSubscription<EventType<?>> sub) {
    return sub.getSymbols().toArray();
  }

  private static Object[] getDecoratedSymbols(DXFeedSubscription<EventType<?>> sub) {
    return sub.getDecoratedSymbols().toArray();
  }

  private static native void nOnEventListener(int size, byte[] byteData, double[] doubleData,
                                              byte[] pEventTypes, long userCallback, long userData);
  private static native void nOnSymbolsAdded(int size, Object[] symbols, long symbolAddedCallback, long userData);
  private static native void nOnSymbolsRemoved(int size, Object[] symbols, long symbolRemovedCallback, long userData);
  private static native void nOnSubscriptionClosed(long subscriptionRemovedCallback, long userData);
}
