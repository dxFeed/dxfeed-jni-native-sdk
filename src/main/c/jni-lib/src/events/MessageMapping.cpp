// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/MessageMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  dxfg_message_t* MessageMapping::toMessage(const char** pByteData) {
    auto* message = new dxfg_message_t();
    message->event_type.clazz = DXFG_EVENT_MESSAGE;
    message->event_symbol = ByteReader::readString(pByteData);
    message->event_time = ByteReader::readLong(pByteData);
    message->attachment = r_cast<const void*>(ByteReader::readString(pByteData));
    return message;
  }

  void MessageMapping::fromMessage(dxfg_message_t* message, ByteWriter& writer) {
    writer.writeString(message->event_symbol);
    writer.writeInt64_t(message->event_time);
    writer.writeString(r_cast<const char*>(message->attachment));
  }
}