package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.DoubleBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.event.market.DxFeedEventMarketPackagePrivate;
import com.dxfeed.event.market.OptionSale;

public class OptionSaleMapping {
  /**
   * typedef struct dxfg_market_event_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
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

  public static void toNative(OptionSale event, ByteBuffer pBytes, DoubleBuffer pDoubles) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());                           // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());                               // 8
    pBytes.writeInt(event.getEventFlags());                               // 4
    pBytes.writeLong(event.getIndex());                                   // 8
    pBytes.writeLong(event.getTimeSequence());                            // 8
    pBytes.writeInt(event.getTimeNanoPart());                             // 4
    pBytes.writeChar(event.getExchangeCode());                            // 2
    pBytes.writeInt(DxFeedEventMarketPackagePrivate.getFlags(event));     // 4
    pBytes.writeString(event.getExchangeSaleConditions());                // 2 + exchangeSaleConditionsLength
    pBytes.writeString(event.getOptionSymbol());                          // 2 + optionSymbolLength

    // DOUBLE DATA
    pDoubles.write(event.getPrice());
    pDoubles.write(event.getSize());
    pDoubles.write(event.getBidPrice());
    pDoubles.write(event.getAskPrice());
    pDoubles.write(event.getUnderlyingPrice());
    pDoubles.write(event.getVolatility());
    pDoubles.write(event.getDelta());
  }

  public static OptionSale fromNative(ByteReader reader) {
    OptionSale optionSale = new OptionSale();
    optionSale.setEventSymbol(reader.readString());
    optionSale.setEventTime(reader.readLong());
    optionSale.setEventFlags(reader.readInt());
    optionSale.setIndex(reader.readLong());
    optionSale.setTimeSequence(reader.readLong());
    optionSale.setTimeNanoPart(reader.readInt());
    optionSale.setExchangeCode(reader.readChar());
    DxFeedEventMarketPackagePrivate.setFlags(optionSale, reader.readInt());
    optionSale.setExchangeSaleConditions(reader.readString());
    optionSale.setOptionSymbol(reader.readString());

    optionSale.setPrice(reader.readDouble());
    optionSale.setSize(reader.readDouble());
    optionSale.setBidPrice(reader.readDouble());
    optionSale.setAskPrice(reader.readDouble());
    optionSale.setUnderlyingPrice(reader.readDouble());
    optionSale.setVolatility(reader.readDouble());
    optionSale.setDelta(reader.readDouble());
    return optionSale;
  }
}
