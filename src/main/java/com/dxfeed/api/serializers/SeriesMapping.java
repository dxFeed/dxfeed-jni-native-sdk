// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.event.option.Series;

public class SeriesMapping {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L438
   *
   * typedef struct dxfg_series_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_indexed_event_t indexed_event;
   *    int32_t event_flags;
   *    int64_t index;
   *    int64_t time_sequence;
   *    int32_t expiration;
   *    double volatility;
   *    double call_volume;
   *    double put_volume;
   *    double put_call_ratio;
   *    double forward_price;
   *    double dividend;
   *    double interest;
   * } dxfg_series_t;
   */

  public static void toNative(Series event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());  // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());      // 8
    pBytes.writeInt(event.getEventFlags());      // 4
    pBytes.writeLong(event.getIndex());          // 8
    pBytes.writeLong(event.getTimeSequence());   // 8
    pBytes.writeInt(event.getExpiration());      // 8

    // DOUBLE DATA
    pDoubles.write(event.getVolatility());
    pDoubles.write(event.getCallVolume());
    pDoubles.write(event.getPutVolume());
    pDoubles.write(event.getPutCallRatio());
    pDoubles.write(event.getForwardPrice());
    pDoubles.write(event.getDividend());
    pDoubles.write(event.getInterest());
  }

  public static Series fromNative(ByteReader reader) {
    Series series = new Series();
    series.setEventSymbol(reader.readString());
    series.setEventTime(reader.readLong());
    series.setEventFlags(reader.readInt());
    series.setIndex(reader.readLong());
    series.setTimeSequence(reader.readLong());
    series.setExpiration(reader.readInt());

    series.setVolatility(reader.readDouble());
    series.setCallVolume(reader.readDouble());
    series.setPutVolume(reader.readDouble());
    series.setPutCallRatio(reader.readDouble());
    series.setForwardPrice(reader.readDouble());
    series.setDividend(reader.readDouble());
    series.setInterest(reader.readDouble());
    return series;
  }
}
