// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/events/ConfigurationMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  dxfg_configuration_t* ConfigurationMapping::toConfiguration(ByteReader& reader) {
    auto* configuration = new dxfg_configuration_t();
    configuration->event_type.clazz = DXFG_EVENT_CONFIGURATION;
    configuration->event_symbol = reader.readString();
    configuration->event_time = reader.readLong();
    configuration->version = reader.readInt();
    configuration->attachment = r_cast<const void*>(reader.readString());
    return configuration;
  }

  void ConfigurationMapping::fromConfiguration(dxfg_configuration_t* configuration, ByteWriter& writer) {
    writer.writeString(configuration->event_symbol);
    writer.writeInt64_t(configuration->event_time);
    writer.writeInt32_t(configuration->version);
    writer.writeString(r_cast<const char*>(configuration->attachment));
  }
}