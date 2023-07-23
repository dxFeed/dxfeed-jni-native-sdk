package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.OptionSale;

public class OptionSaleToNative {
  /**
   * typedef struct dxfg_market_event_t {
   * dxfg_event_type_t event_type;
   * const char *event_symbol;
   * int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L360
   *
   * typedef struct dxfg_option_sale_t {
   *    dxfg_market_event_t market_event;
   *    //    dxfg_indexed_event_t indexed_event;
   *    int32_t event_flags;
   *    int64_t index;
   *    int64_t time_sequence;
   *    int32_t time_nano_part;
   *    int16_t exchange_code;
   *    double price;
   *    double size;
   *    double bid_price;
   *    double ask_price;
   *    const char *exchange_sale_conditions;
   *    int32_t flags;
   *    double underlying_price;
   *    double volatility;
   *    double delta;
   *    const char *option_symbol;
   * } dxfg_option_sale_t;
   */

  public static void convert(OptionSale event, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                                      // 8
    int eventFlags = event.getEventFlags();                                     // 4
    long index = event.getIndex();                                              // 8
    long timeSequence = event.getTimeSequence();                                // 8
    int timeNanoPart = event.getTimeNanoPart();                                 // 4
    char exchangeCode = event.getExchangeCode();                                // 2
    int flags = DxFeedEventMarketPackagePrivate.getFlags(event);                // 4
    CString exchangeSaleConditions = new CString(event.getExchangeSaleConditions());
    CString optionSymbol = new CString(event.getOptionSymbol());

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8) + (8) + (4) + (2) + (4) +
            exchangeSaleConditions.totalAllocatedBytes + optionSymbol.totalAllocatedBytes;

    pBytes.addChunk(eventIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(index);                    // 8
    pBytes.writeLong(timeSequence);             // 8
    pBytes.writeInt(timeNanoPart);              // 4
    pBytes.writeChar(exchangeCode);             // 2
    pBytes.writeInt(flags);                     // 4
    pBytes.writeString(exchangeSaleConditions);
    pBytes.writeString(optionSymbol);

    double price = event.getPrice();                      // 1
    double size = event.getSize();                        // 1
    double bidPrice = event.getBidPrice();                // 1
    double askPrice = event.getAskPrice();                // 1
    double underlyingPrice = event.getUnderlyingPrice();  // 1
    double volatility = event.getVolatility();            // 1
    double delta = event.getDelta();                      // 1

    // DOUBLE DATA
    pDoubles.addChunk(eventIdx, 7);
    pDoubles.write(price);
    pDoubles.write(size);
    pDoubles.write(bidPrice);
    pDoubles.write(askPrice);
    pDoubles.write(underlyingPrice);
    pDoubles.write(volatility);
    pDoubles.write(delta);
  }
}
