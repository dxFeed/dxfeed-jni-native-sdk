// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/events/MessageMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  dxfg_message_t* MessageMapping::toMessage(ByteReader& reader) {
    auto* message = new dxfg_message_t();
    message->event_type.clazz = DXFG_EVENT_MESSAGE;
    message->event_symbol = reader.readString();
    message->event_time = reader.readLong();
    message->attachment = r_cast<const void*>(reader.readString());
    return message;
  }

  void MessageMapping::fromMessage(dxfg_message_t* message, ByteWriter& writer) {
    writer.writeString(message->event_symbol);
    writer.writeInt64_t(message->event_time);
    writer.writeString(r_cast<const char*>(message->attachment));
  }
}