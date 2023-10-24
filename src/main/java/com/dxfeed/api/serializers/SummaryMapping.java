
package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Summary;

public class SummaryMapping {
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

  public static void toNative(Summary event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                       // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                           // 8
    pBytes.writeInt(event.getDayId());                                // 4
    pBytes.writeInt(event.getPrevDayId());                            // 4
    pBytes.writeLong(event.getOpenInterest());                        // 8
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getFlags(event)); // 4

    // DOUBLE DATA
    pDoubles.write(event.getDayOpenPrice());
    pDoubles.write(event.getDayHighPrice());
    pDoubles.write(event.getDayLowPrice());
    pDoubles.write(event.getDayClosePrice());
    pDoubles.write(event.getPrevDayClosePrice());
    pDoubles.write(event.getPrevDayVolume());
  }

  public static Summary fromNative(ByteReader reader) {
    Summary summary = new Summary();
    summary.setEventSymbol(reader.readString());
    summary.setEventTime(reader.readLong());
    summary.setDayId(reader.readInt());
    summary.setPrevDayId(reader.readInt());
    summary.setOpenInterest(reader.readLong());
    DxFeedEventMarketPackagePrivate.setFlags(summary, reader.readInt());

    summary.setDayOpenPrice(reader.readDouble());
    summary.setDayHighPrice(reader.readDouble());
    summary.setDayLowPrice(reader.readDouble());
    summary.setDayClosePrice(reader.readDouble());
    summary.setPrevDayClosePrice(reader.readDouble());
    summary.setPrevDayVolume(reader.readDouble());
    return summary;
  }
}
