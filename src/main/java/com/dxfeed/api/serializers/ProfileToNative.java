package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Profile;

public class ProfileToNative {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L158
   *
   * typedef struct dxfg_profile_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_lasting_event_t lasting_event;
   *    const char *description;
   *    const char *status_reason;
   *    int64_t halt_start_time;
   *    int64_t halt_end_time;
   *    double high_limit_price;
   *    double low_limit_price;
   *    double high_52_week_price;
   *    double low_52_week_price;
   *    double beta;
   *    double earnings_per_share;
   *    double dividend_frequency;
   *    double ex_dividend_amount;
   *    int32_t ex_dividend_day_id;
   *    double shares;
   *    double free_float;
   *    int32_t flags;
   * } dxfg_profile_t;
   */
  public static void convert(Profile event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                       // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                           // 8
    pBytes.writeLong(event.getHaltStartTime());                       // 8
    pBytes.writeLong(event.getHaltEndTime());                         // 8
    pBytes.writeInt(event.getExDividendDayId());                      // 4
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getFlags(event)); // 4
    pBytes.writeString(event.getDescription());                       // 2 + descriptionLength
    pBytes.writeString(event.getStatusReason());                      // 2 + statusReasonLength

    // DOUBLE DATA
    pDoubles.write(event.getHighLimitPrice());
    pDoubles.write(event.getLowLimitPrice());
    pDoubles.write(event.getHigh52WeekPrice());
    pDoubles.write(event.getLow52WeekPrice());
    pDoubles.write(event.getBeta());
    pDoubles.write(event.getEarningsPerShare());
    pDoubles.write(event.getDividendFrequency());
    pDoubles.write(event.getExDividendAmount());
    pDoubles.write(event.getShares());
    pDoubles.write(event.getFreeFloat());
  }

  public static Profile fromNative(NativeEventsReader eventsReader) {
    Profile profile = new Profile();

    profile.setEventSymbol(eventsReader.readString());
    profile.setEventTime(eventsReader.readLong());
    profile.setHaltStartTime(eventsReader.readLong());
    profile.setHaltEndTime(eventsReader.readLong());
    profile.setExDividendDayId(eventsReader.readInt());
    DxFeedEventMarketPackagePrivate.setFlags(profile, eventsReader.readInt());
    profile.setDescription(eventsReader.readString());
    profile.setStatusReason(eventsReader.readString());

    profile.setHighLimitPrice(eventsReader.readDouble());
    profile.setLowLimitPrice(eventsReader.readDouble());
    profile.setHigh52WeekPrice(eventsReader.readDouble());
    profile.setLow52WeekPrice(eventsReader.readDouble());
    profile.setBeta(eventsReader.readDouble());
    profile.setEarningsPerShare(eventsReader.readDouble());
    profile.setDividendFrequency(eventsReader.readDouble());
    profile.setExDividendAmount(eventsReader.readDouble());
    profile.setShares(eventsReader.readDouble());
    profile.setFreeFloat(eventsReader.readDouble());

    return profile;
  }
}
