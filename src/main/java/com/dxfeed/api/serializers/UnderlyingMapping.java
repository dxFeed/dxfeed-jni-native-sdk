// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.event.option.Underlying;

public class UnderlyingMapping {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L250
   *
   * typedef struct dxfg_underlying_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_time_series_event_t time_series_event;
   *    //    dxfg_lasting_event_t lasting_event;
   *    int32_t event_flags;
   *    int64_t index;
   *    double volatility;
   *    double front_volatility;
   *    double back_volatility;
   *    double call_volume;
   *    double put_volume;
   *    double put_call_ratio;
   * } dxfg_underlying_t;
   */

  public static void toNative(Underlying event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());   // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());       // 8
    pBytes.writeInt(event.getEventFlags());       // 4
    pBytes.writeLong(event.getIndex());           // 8

    // DOUBLE DATA
    pDoubles.write(event.getVolatility());
    pDoubles.write(event.getFrontVolatility());
    pDoubles.write(event.getBackVolatility());
    pDoubles.write(event.getCallVolume());
    pDoubles.write(event.getPutVolume());
    pDoubles.write(event.getPutCallRatio());
  }

  public static Underlying fromNative(ByteReader reader) {
    Underlying underlying = new Underlying();
    underlying.setEventSymbol(reader.readString());
    underlying.setEventTime(reader.readLong());
    underlying.setEventFlags(reader.readInt());
    underlying.setIndex(reader.readLong());

    underlying.setVolatility(reader.readDouble());
    underlying.setFrontVolatility(reader.readDouble());
    underlying.setBackVolatility(reader.readDouble());
    underlying.setCallVolume(reader.readDouble());
    underlying.setPutVolume(reader.readDouble());
    underlying.setPutCallRatio(reader.readDouble());
    return underlying;
  }
}
