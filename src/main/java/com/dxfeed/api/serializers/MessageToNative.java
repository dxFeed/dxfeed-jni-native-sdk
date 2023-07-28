package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.event.misc.Message;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

public class MessageToNative {
  private static final Map<Long, Object> attachmentMap = new HashMap<>();
  private static final AtomicLong attachmentId = new AtomicLong();

  // todo: get attachment by ID using JNI

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

  public static void convert(Message event, ByteBuffer pBytes) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol()); // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());     // 8
    long id = attachmentId.incrementAndGet();
    pBytes.writeLong(id);                       // 8
    attachmentMap.putIfAbsent(id, event.getAttachment());
  }
}
