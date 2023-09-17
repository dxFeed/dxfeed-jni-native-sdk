package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ByteBuffer;
import com.dxfeed.api.buffers.NativeEventsReader;
import com.dxfeed.api.utils.XmlMapping;
import com.dxfeed.event.misc.Configuration;

public class ConfigurationMapping {

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
    pBytes.writeString(event.getEventSymbol());  // 2 + eventSymbolLength
    pBytes.writeLong(event.getEventTime());      // 8
    pBytes.writeInt(event.getVersion());         // 4
    pBytes.writeString(XmlMapping.toString(event.getAttachment()));
  }

  public static Configuration fromNative(NativeEventsReader reader) {
    Configuration configuration = new Configuration();
    configuration.setEventSymbol(reader.readString());
    configuration.setEventTime(reader.readLong());
    configuration.setVersion(reader.readInt());
    // todo: attachment always is null ?
    Object attachment = configuration.getAttachment();
    if (attachment != null) {
      configuration.setAttachment(XmlMapping.fromString(reader.readString(), attachment.getClass()));
    }
    return configuration;
  }
}
