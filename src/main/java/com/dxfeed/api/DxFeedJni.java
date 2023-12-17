// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api;

import com.dxfeed.event.IndexedEvent;
import com.dxfeed.event.LastingEvent;
import com.dxfeed.event.TimeSeriesEvent;
import com.dxfeed.event.market.OrderSource;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;

public class DxFeedJni {

  public static final ConcurrentHashMap<Long, Object> nativeObjectsMap = new ConcurrentHashMap<>();
  private static final AtomicLong nativeHandleId = new AtomicLong();

  public static long nextHandleId() {
    return nativeHandleId.incrementAndGet();
  }

  private static void loadLibrary(String libPath) {
    System.load(libPath);
  }

  private static <E extends LastingEvent<?>> NativeEventsList<E> getLastEventIfSubscribed(DXFeed feed,
                                                                                          Class<E> eventTypeClass,
                                                                                          Object symbol)
  {
    E lastEvent = feed.getLastEventIfSubscribed(eventTypeClass, symbol);
    return (lastEvent != null) ? new NativeEventsList<>(Collections.singletonList(lastEvent)) : null;
  }

  private static <E extends LastingEvent<?>> NativeEventsList<E> getLastEvents(DXFeed feed,
                                                                               Class<E>[] eventTypeClassList,
                                                                               String[] eventNameList)
  {
    assert eventTypeClassList.length == eventNameList.length;
    int size = eventNameList.length;
    List<E> events = new ArrayList<>();
    for (int i = 0; i < size; ++i) {
      Class<E> eventTypeClass = eventTypeClassList[i];
      String eventName = eventNameList[i];
      E event = createEventByClass(eventTypeClass, eventName);
      events.add(event);
    }
    feed.getLastEvents(events);
    return new NativeEventsList<>(events);
  }

  private static <E extends LastingEvent<?>> E createEventByClass(Class<E> eventTypeClass, String eventName) {
    try {
      Constructor<E> constructor = eventTypeClass.getConstructor(String.class);
      return constructor.newInstance(eventName);
    } catch (NoSuchMethodException | InstantiationException | IllegalAccessException | InvocationTargetException e) {
      throw new RuntimeException(e);
    }
  }

  private static NativeEventsList<?> getIndexedEventsIfSubscribed(DXFeed feed, Class<IndexedEvent<?>> eventTypeClass,
                                                                  Object symbol, String source)
  {
    List<IndexedEvent<?>> list = feed.getIndexedEventsIfSubscribed(eventTypeClass, symbol, OrderSource.valueOf(source));
    return new NativeEventsList<>(list);
  }

  private static NativeEventsList<?> getTimeSeriesIfSubscribed(DXFeed feed, Class<TimeSeriesEvent<?>> eventTypeClass,
                                                               Object symbol, long fromTime, long toTime)
  {
    List<TimeSeriesEvent<?>> list = feed.getTimeSeriesIfSubscribed(eventTypeClass, symbol, fromTime, toTime);
    return new NativeEventsList<>(list);
  }
}
