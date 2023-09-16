package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.Profile;

public class ProfileMapping {
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
  public static void toNative(Profile event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
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

  public static Profile fromNative(NativeEventsReader reader) {
    Profile profile = new Profile();
    profile.setEventSymbol(reader.readString());
    profile.setEventTime(reader.readLong());
    profile.setHaltStartTime(reader.readLong());
    profile.setHaltEndTime(reader.readLong());
    profile.setExDividendDayId(reader.readInt());
    DxFeedEventMarketPackagePrivate.setFlags(profile, reader.readInt());
    profile.setDescription(reader.readString());
    profile.setStatusReason(reader.readString());

    profile.setHighLimitPrice(reader.readDouble());
    profile.setLowLimitPrice(reader.readDouble());
    profile.setHigh52WeekPrice(reader.readDouble());
    profile.setLow52WeekPrice(reader.readDouble());
    profile.setBeta(reader.readDouble());
    profile.setEarningsPerShare(reader.readDouble());
    profile.setDividendFrequency(reader.readDouble());
    profile.setExDividendAmount(reader.readDouble());
    profile.setShares(reader.readDouble());
    profile.setFreeFloat(reader.readDouble());
    return profile;
  }
}
