package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.misc.Configuration;
import com.dxfeed.event.option.Underlying;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicLong;

public class ConfigurationToNative {
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

  public static void convert(Configuration event, ChunkedByteBuffer pBytes, int eventIdx) {
    CString eventSymbol = new CString(event.getEventSymbol());
    long eventTime = event.getEventTime();                      // 8
    int eventFlags = event.getVersion();                        // 4
    long id = attachmentId.incrementAndGet();
    attachmentMap.putIfAbsent(id, event.getAttachment());       // 8

    int totalSize = eventSymbol.totalAllocatedBytes + (8) + (4) + (8);

    pBytes.addChunk(eventIdx, totalSize);
    pBytes.writeString(eventSymbol);
    pBytes.writeLong(eventTime);                // 8
    pBytes.writeInt(eventFlags);                // 4
    pBytes.writeLong(id);                       // 8
  }
}
