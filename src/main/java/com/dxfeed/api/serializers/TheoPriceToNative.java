
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.event.option.TheoPrice;

public class TheoPriceToNative {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L268
   *
   * typedef struct dxfg_theo_price_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_time_series_event_t time_series_event;
   *    //    dxfg_lasting_event_t lasting_event;
   *    int32_t event_flags;
   *
   *    int64_t index;
   *    double price;
   *    double underlying_price;
   *    double delta;
   *    double gamma;
   *    double dividend;
   *    double interest;
   * } dxfg_theo_price_t;
   */

  public static void convert(TheoPrice event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());   // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());       // 8
    pBytes.writeInt(event.getEventFlags());       // 4
    pBytes.writeLong(event.getIndex());           // 8

    // DOUBLE DATA
    pDoubles.write(event.getPrice());
    pDoubles.write(event.getUnderlyingPrice());
    pDoubles.write(event.getDelta());
    pDoubles.write(event.getGamma());
    pDoubles.write(event.getDividend());
    pDoubles.write(event.getInterest());
  }
}
