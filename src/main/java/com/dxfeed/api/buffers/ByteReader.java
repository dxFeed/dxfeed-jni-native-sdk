// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api.buffers;


import com.dxfeed.api.DxfgEventClazzT;
import com.dxfeed.api.serializers.*;
import com.dxfeed.event.EventType;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public class ByteReader {
  private final byte[] byteData;
  private final double[] doubleData;
  private final byte[] eventTypes;
  private int bytePos = 0;
  private int doublePos = 0;

  public ByteReader(byte[] byteData, double[] doubleData, byte[] eventTypes) {
    this.byteData = byteData;
    this.doubleData = doubleData;
    this.eventTypes = eventTypes;
  }

  private byte readByte() {
    return byteData[bytePos++];
  }

  public String readString() {
    char size = readChar();
    String str = new String(byteData, bytePos, size, StandardCharsets.UTF_8);
    bytePos += size;
    return str;
  }

  public short readShort() {
    short value = 0;
    for (int i = 0; i < Short.BYTES; ++i) {
      value <<= 8;
      value |= readByte() & 0xFF;
    }
    return value;
  }

  public char readChar() {
    char value = 0;
    for (int i = 0; i < Character.BYTES; ++i) {
      value <<= 8;
      value |= readByte() & 0xFF;
    }
    return value;
  }

  public int readInt() {
    int value = 0;
    for (int i = 0; i < Integer.BYTES; ++i) {
      value <<= 8;
      value |= readByte() & 0xFF;
    }
    return value;
  }

  public long readLong() {
    long value = 0;
    for (int i = 0; i < Long.BYTES; ++i) {
      value <<= 8;
      value |= readByte() & 0xFF;
    }
    return value;
  }

  public double readDouble() {
    return doubleData[doublePos++];
  }

  public List<EventType<?>> toEvents() {
    List<EventType<?>> eventList = new ArrayList<>();
    for (byte pEventType : eventTypes) {
      eventList.add(readEvent(pEventType));
    }
    return eventList;
  }

  private EventType<?> readEvent(byte pEventType) {
    switch (pEventType) {
      case DxfgEventClazzT.DXFG_EVENT_QUOTE: {
        return QuoteMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_PROFILE: {
        return ProfileMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_SUMMARY: {
        return SummaryMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_GREEKS: {
        return GreeksMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_CANDLE: {
        return CandleMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_UNDERLYING: {
        return UnderlyingMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_THEO_PRICE: {
        return TheoPriceMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_TRADE:
      case DxfgEventClazzT.DXFG_EVENT_TRADE_ETH: {
        return TradeMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_CONFIGURATION: {
        return ConfigurationMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_MESSAGE: {
        MessageMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_TIME_AND_SALE: {
        return TimeAndSalesMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_ORDER_BASE: {
        return OrderToMapping.orderBaseFromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_ORDER: {
        return OrderToMapping.orderFromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_ANALYTIC_ORDER: {
        return OrderToMapping.analyticOrderFromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_SPREAD_ORDER: {
        return OrderToMapping.spreadOrderFromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_OPTION_SALE: {
        return OptionSaleMapping.fromNative(this);
      }
      case DxfgEventClazzT.DXFG_EVENT_SERIES: {
        return SeriesMapping.fromNative(this);
      }
      default:
        throw new IllegalStateException("Event mapping for event type " + pEventType + " is not implemented");
    }
  }
}
