package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.option.Series;

public class SeriesToNative {
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

  public static void convert(Series event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                     // 8
    int eventFlags = event.getEventFlags();                    // 4
    long index = event.getIndex();                             // 8
    long timeSequence = event.getTimeSequence();               // 8
    int expiration = event.getExpiration();                    // 8

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8) + (8) + (8);

    pBytes.addChunk(eventIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8
    pBytes.writeLong(timeSequence);             // 8
    pBytes.writeInt(expiration);                // 8

    double volatility = event.getVolatility();        // 1
    double callVolume = event.getCallVolume();        // 1
    double putVolume = event.getPutVolume();          // 1
    double putCallRatio = event.getPutCallRatio();    // 1
    double forwardPrice = event.getForwardPrice();    // 1
    double dividend = event.getDividend();            // 1
    double interest = event.getInterest();            // 1

    // DOUBLE DATA
    pDoubles.addChunk(eventIdx, 7);
    pDoubles.write(volatility);
    pDoubles.write(callVolume);
    pDoubles.write(putVolume);
    pDoubles.write(putCallRatio);
    pDoubles.write(forwardPrice);
    pDoubles.write(dividend);
    pDoubles.write(interest);
  }
}
