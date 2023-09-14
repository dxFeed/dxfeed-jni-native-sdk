package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.event.misc.Configuration;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

public class ConfigurationMapping {
  private static final Map<Long, Object> attachmentMap = new HashMap<>();
  private static final AtomicLong attachmentId = new AtomicLong();

  // todo: get attachment by ID using JNI

  /**
   * https://github.com/dxFeed/dxfeed-graal-native-sdk/blob/main/src/main/c/api/dxfg_events.h#L318
   *
   * typedef struct dxfg_configuration_t {
   *    dxfg_event_type_t event_type;
   *    //    dxfg_lasting_event_t lasting_event;
   *    const char *event_symbol;
   *    int64_t event_time;
   *    int32_t version;
   *    void *attachment;
   * } dxfg_configuration_t;
   */

  public static void toNative(Configuration event, ByteBuffer pBytes) {
    // BYTE DATA
    pBytes.writeString(event.getEventSymbol());  // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());      // 8
    pBytes.writeInt(event.getVersion());         // 4
    long id = attachmentId.incrementAndGet();
    pBytes.writeLong(id);                        // 8
    attachmentMap.putIfAbsent(id, event.getAttachment());
  }
}
