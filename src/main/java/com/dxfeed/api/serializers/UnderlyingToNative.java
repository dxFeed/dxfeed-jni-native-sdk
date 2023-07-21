
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.option.Underlying;

public class UnderlyingToNative {
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

  public static void convert(Underlying event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
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

    double volatility = event.getVolatility();            // 1
    double frontVolatility = event.getFrontVolatility();  // 1
    double backVolatility = event.getBackVolatility();    // 1
    double callVolume = event.getCallVolume();            // 1
    double pullVolume = event.getPutVolume();             // 1
    double putCallVolume = event.getPutCallRatio();       // 1

    // DOUBLE DATA
    pDoubles.addChunk(eventIdx, 6);
    pDoubles.write(volatility);
    pDoubles.write(frontVolatility);
    pDoubles.write(backVolatility);
    pDoubles.write(callVolume);
    pDoubles.write(pullVolume);
    pDoubles.write(putCallVolume);
  }
}
