package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.TimeAndSale;

public class TimeAndSalesMapping {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   * } dxfg_market_event_t;
   *
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L340
   *
   * typedef struct dxfg_time_and_sale_t {
   *    dxfg_market_event_t market_event;
   *    int32_t event_flags;
   *    int64_t index;
   *    int32_t time_nano_part;
   *    int16_t exchange_code;
   *    double price;
   *    double size;
   *    double bid_price;
   *    double ask_price;
   *    const char *exchange_sale_conditions;
   *    int32_t flags;
   *    const char *buyer;
   *    const char *seller;
   * } dxfg_time_and_sale_t;
   */

  public static void toNative(TimeAndSale event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                         // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                             // 8
    pBytes.writeInt(event.getEventFlags());                             // 4
    pBytes.writeLong(event.getIndex());                                 // 8
    pBytes.writeInt(event.getTimeNanoPart());                           // 4
    pBytes.writeChar(event.getExchangeCode());                          // 2
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getFlags(event));   // 4
    pBytes.writeString(event.getExchangeSaleConditions());              // 2 + exchangeSaleConditionsLength
    pBytes.writeString(event.getBuyer());                               // 2 + buyerLength
    pBytes.writeString(event.getSeller());                              // 2 + sellerLength

    // DOUBLE DATA
    pDoubles.write(event.getPrice());
    pDoubles.write(event.getSize());
    pDoubles.write(event.getBidPrice());
    pDoubles.write(event.getAskPrice());
  }

  public static TimeAndSale fromNative(NativeEventsReader reader) {
    TimeAndSale tns = new TimeAndSale();

    tns.setEventSymbol(reader.readString());
    tns.setEventTime(reader.readLong());
    tns.setEventFlags(reader.readInt());
    tns.setIndex(reader.readLong());
    tns.setTimeNanoPart(reader.readInt());
    tns.setExchangeCode(reader.readChar());
    DxFeedEventMarketPackagePrivate.setFlags(tns, reader.readInt());
    tns.setExchangeSaleConditions(reader.readString());
    tns.setBuyer(reader.readString());
    tns.setSeller(reader.readString());

    tns.setPrice(reader.readDouble());
    tns.setSizeAsDouble(reader.readDouble());
    tns.setBidPrice(reader.readDouble());
    tns.setAskPrice(reader.readDouble());

    return tns;
  }
}
