
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.option.TheoPrice;
import com.dxfeed.event.option.Underlying;

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

  public static void convert(TheoPrice event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                      // 8
    int eventFlags = event.getEventFlags();                                     // 4
    long index = event.getIndex();                                              // 8

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8);

    pBytes.addChunk(eventIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8

    double price = event.getPrice();                      // 1
    double underlyingPrice = event.getUnderlyingPrice();  // 1
    double delta = event.getDelta();                      // 1
    double gamma = event.getGamma();                      // 1
    double dividend = event.getDividend();                // 1
    double interest = event.getInterest();                // 1

    // DOUBLE DATA
    pDoubles.addChunk(eventIdx, 6);
    pDoubles.write(price);
    pDoubles.write(underlyingPrice);
    pDoubles.write(delta);
    pDoubles.write(gamma);
    pDoubles.write(dividend);
    pDoubles.write(interest);
  }
}
