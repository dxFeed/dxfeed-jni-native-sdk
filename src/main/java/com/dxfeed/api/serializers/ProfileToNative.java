package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.DxFeedEventMarket;
import com.dxfeed.event.market.Profile;

public class ProfileToNative {
  /**
   * typedef struct dxfg_market_event_t {
   * dxfg_event_type_t event_type;
   * const char *event_symbol;
   * int64_t event_time;
   * } dxfg_market_event_t;
   * <p>
   * // https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L158
   * typedef struct dxfg_profile_t {
   * dxfg_market_event_t market_event;
   * //    dxfg_lasting_event_t lasting_event;
   * const char *description;
   * const char *status_reason;
   * int64_t halt_start_time;
   * int64_t halt_end_time;
   * double high_limit_price;
   * double low_limit_price;
   * double high_52_week_price;
   * double low_52_week_price;
   * double beta;
   * double earnings_per_share;
   * double dividend_frequency;
   * double ex_dividend_amount;
   * int32_t ex_dividend_day_id;
   * double shares;
   * double free_float;
   * int32_t flags;
   * } dxfg_profile_t;
   */
  public static void convert(Profile event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int chunkIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                  // 8
    long haltStartTime = event.getHaltStartTime();                          // 8
    long haltEndTime = event.getHaltEndTime();                              // 8
    int exDividendDayId = event.getExDividendDayId();                       // 4
    int flags = DxFeedEventMarket.ProfilePackagePrivate.getFlags(event);    // 4
    CString description = new CString(event.getDescription());
    CString statusReason = new CString(event.getStatusReason());

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (8) + (8) + (4) + (4) +
        description.totalAllocatedBytes + statusReason.totalAllocatedBytes;

    pBytes.addChunk(chunkIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);            // 8
    pBytes.writeLong(haltStartTime);        // 8
    pBytes.writeLong(haltEndTime);          // 8
    pBytes.writeInt(exDividendDayId);       // 4
    pBytes.writeInt(flags);                 // 4
    pBytes.writeString(description);
    pBytes.writeString(statusReason);

    // DOUBLE DATA
    double highLimitPrice = event.getHighLimitPrice();          // 1
    double lowLimitPrice = event.getLowLimitPrice();            // 1
    double high52WeekPrice = event.getHigh52WeekPrice();        // 1
    double low52WeekPrice = event.getLow52WeekPrice();          // 1
    double beta = event.getBeta();                              // 1
    double earningsPerShare = event.getEarningsPerShare();      // 1
    double dividendFrequency = event.getDividendFrequency();    // 1
    double dividendAmount = event.getExDividendAmount();        // 1
    double shares = event.getShares();                          // 1
    double freeFloat = event.getFreeFloat();                    // 1

    // DOUBLE DATA
    pDoubles.addChunk(chunkIdx, 10);
    pDoubles.write(highLimitPrice);
    pDoubles.write(lowLimitPrice);
    pDoubles.write(high52WeekPrice);
    pDoubles.write(low52WeekPrice);
    pDoubles.write(beta);
    pDoubles.write(earningsPerShare);
    pDoubles.write(dividendFrequency);
    pDoubles.write(dividendAmount);
    pDoubles.write(shares);
    pDoubles.write(freeFloat);
  }
}
