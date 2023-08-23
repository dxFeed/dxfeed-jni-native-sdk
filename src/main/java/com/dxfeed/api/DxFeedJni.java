package com.dxfeed.api;

import com.dxfeed.event.IndexedEvent;
import com.dxfeed.event.LastingEvent;
import com.dxfeed.event.TimeSeriesEvent;
import com.dxfeed.event.market.OrderSource;
import com.dxfeed.event.market.Quote;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
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
                                                                                      Object symbol) {
    E lastEvent = feed.getLastEventIfSubscribed(eventTypeClass, symbol);
    System.out.println("DxFeedJni::getLastEventIfSubscribed = " + lastEvent);
    return (lastEvent != null) ? new NativeEventsList<>(Collections.singletonList(lastEvent)) : null;
  }

  private static <E extends LastingEvent<?>> NativeEventsList<E> getLastEvent(DXFeed feed, Class<E> eventTypeClass,
                                                                              String eventName) {
    E event = createEventByClass(eventTypeClass, eventName);
    System.out.println("event before getLastEvent = " + event);
    feed.getLastEvent(event);
    System.out.println("event after getLastEvent = " + event);
    if (event instanceof Quote) {
      return new NativeEventsList<>(Collections.singletonList(event));
    }
    return null;
  }

  private static <E extends LastingEvent<?>> E createEventByClass(Class<E> eventTypeClass, String eventName) {
    try {
      Constructor<E> constructor = eventTypeClass.getConstructor(String.class);
      System.out.println("constructor = " + constructor);
      E event = constructor.newInstance(eventName);
      System.out.println("event = " + event);
      return event;
    } catch (NoSuchMethodException | InstantiationException | IllegalAccessException | InvocationTargetException e) {
      throw new RuntimeException(e);
    }
  }

  private static NativeEventsList<?> getIndexedEventsIfSubscribed(DXFeed feed, Class<IndexedEvent<?>> eventTypeClass,
                                                                  Object symbol, String source)
  {
    List<IndexedEvent<?>> list = feed.getIndexedEventsIfSubscribed(eventTypeClass, symbol, OrderSource.valueOf(source));
    System.out.println("DxFeedJni::getIndexedEventsIfSubscribed, events count = " + list.toString());
    return new NativeEventsList<>(list);
  }

  private static NativeEventsList<?> getTimeSeriesIfSubscribed(DXFeed feed, Class<TimeSeriesEvent<?>> eventTypeClass,
                                                               Object symbol, long fromTime, long toTime)
  {
    List<TimeSeriesEvent<?>> list = feed.getTimeSeriesIfSubscribed(eventTypeClass, symbol, fromTime, toTime);
    System.out.println("DxFeedJni::getTimeSeriesIfSubscribed, events count = " + list.toString());
    return new NativeEventsList<>(list);
  }
}
