
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.option.Greeks;

public class GreeksToNative {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L200
   *
   * typedef struct dxfg_greeks_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_time_series_event_t time_series_event;
   *    //    dxfg_lasting_event_t lasting_event;
   *    int32_t event_flags;
   *    int64_t index;
   *    double price;
   *    double volatility;
   *    double delta;
   *    double gamma;
   *    double theta;
   *    double rho;
   *    double vega;
   * } dxfg_greeks_t;
   */

  public static void convert(Greeks event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                     // 8
    int eventFlags = event.getEventFlags();                    // 4
    long index = event.getIndex();                             // 8

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8);

    pBytes.addChunk(eventIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8

    double price = event.getPrice();              // 1
    double volatility = event.getVolatility();    // 1
    double delta = event.getDelta();              // 1
    double gamma = event.getGamma();              // 1
    double theta = event.getTheta();              // 1
    double rho = event.getRho();                  // 1
    double vega = event.getVega();                // 1

    // DOUBLE DATA
    pDoubles.addChunk(eventIdx, 7);
    pDoubles.write(price);
    pDoubles.write(volatility);
    pDoubles.write(delta);
    pDoubles.write(gamma);
    pDoubles.write(theta);
    pDoubles.write(rho);
    pDoubles.write(vega);
  }
}
