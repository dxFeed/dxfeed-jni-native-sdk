
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
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

  public static void convert(Greeks event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol()); // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());     // 8
    pBytes.writeInt(event.getEventFlags());     // 4
    pBytes.writeLong(event.getIndex());         // 8

    // DOUBLE DATA
    pDoubles.write(event.getPrice());
    pDoubles.write(event.getVolatility());
    pDoubles.write(event.getDelta());
    pDoubles.write(event.getGamma());
    pDoubles.write(event.getTheta());
    pDoubles.write(event.getRho());
    pDoubles.write(event.getVega());
  }

  public static Greeks fromNative(NativeEventsReader reader) {
    Greeks greeks = new Greeks();

    greeks.setEventSymbol(reader.readString());
    greeks.setEventTime(reader.readLong());
    greeks.setEventFlags(reader.readInt());
    greeks.setIndex(reader.readLong());

    greeks.setPrice(reader.readDouble());
    greeks.setVolatility(reader.readDouble());
    greeks.setDelta(reader.readDouble());
    greeks.setGamma(reader.readDouble());
    greeks.setTheta(reader.readDouble());
    greeks.setRho(reader.readDouble());
    greeks.setVega(reader.readDouble());

    return greeks;
  }
}
