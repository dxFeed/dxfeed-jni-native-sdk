
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Summary;

public class SummaryToNative {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L182
   *
   * typedef struct dxfg_summary_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_lasting_event_t lasting_event;
   *    int32_t day_id;
   *    double day_open_price;
   *    double day_high_price;
   *    double day_low_price;
   *    double day_close_price;
   *    int32_t prev_day_id;
   *    double prev_day_close_price;
   *    double prev_day_volume;
   *    int64_t open_interest;
   *    int32_t flags;
   * } dxfg_summary_t;
   */

  public static void convert(Summary event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                            // 8
    int dayId = event.getDayId();                                     // 4
    int prevDayId = event.getPrevDayId();                             // 4
    long openInterest = event.getOpenInterest();                      // 8
    int index = DxFeedEventMarketPackagePrivate.getFlags(event);      // 4

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (4) + (8) + (4);

    pBytes.addChunk(eventIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(dayId);                     // 4
    pBytes.writeInt(prevDayId);                 // 4
    pBytes.writeLong(openInterest);             // 8
    pBytes.writeInt(index);                     // 4

    double dayOpenPrice = event.getDayOpenPrice();              // 1
    double dayHighPrice = event.getDayHighPrice();              // 1
    double dayLowPrice = event.getDayLowPrice();                // 1
    double dayClosePrice = event.getDayClosePrice();            // 1
    double prevDayClosePrice = event.getPrevDayClosePrice();    // 1
    double prevDayVolume = event.getPrevDayVolume();            // 1

    // DOUBLE DATA
    pDoubles.addChunk(eventIdx, 6);
    pDoubles.write(dayOpenPrice);
    pDoubles.write(dayHighPrice);
    pDoubles.write(dayLowPrice);
    pDoubles.write(dayClosePrice);
    pDoubles.write(prevDayClosePrice);
    pDoubles.write(prevDayVolume);
  }
}
