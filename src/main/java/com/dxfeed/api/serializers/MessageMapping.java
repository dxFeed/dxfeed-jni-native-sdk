package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.ByteReader;
import com.dxfeed.api.utils.XmlMapping;
import com.dxfeed.event.misc.Message;

public class MessageMapping {
  /**
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L330
   *
   * typedef struct dxfg_message_t {
   *    dxfg_event_type_t event_type;
   *    const char *event_symbol;
   *    int64_t event_time;
   *    void *attachment;
   * } dxfg_message_t;
   */

  public static void toNative(Message event, ByteBuffer pBytes) {
    pBytes.writeString(event.getEventSymbol()); // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());     // 8
    pBytes.writeString(XmlMapping.toString(event.getAttachment()));
  }

  public static Message fromNative(ByteReader reader) {
    Message message = new Message();
    message.setEventSymbol(reader.readString());
    message.setEventTime(reader.readLong());
    // todo: attachment always is null ?
    Object attachment = message.getAttachment();
    if (attachment != null) {
      message.setAttachment(XmlMapping.fromString(reader.readString(), attachment.getClass()));
    }
    return message;
  }
}
