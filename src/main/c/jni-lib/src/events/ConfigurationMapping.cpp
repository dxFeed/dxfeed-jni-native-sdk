// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/ConfigurationMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"
#include "dxfeed/utils/JNIUtils.hpp"

namespace dxfeed::jni {
  dxfg_configuration_t* ConfigurationMapping::toConfiguration(const char** pByteData) {
    auto* configuration = new dxfg_configuration_t();
    configuration->event_type.clazz = DXFG_EVENT_CONFIGURATION;
    configuration->event_symbol = ByteReader::readString(pByteData);
    configuration->event_time = ByteReader::readLong(pByteData);
    configuration->version = ByteReader::readInt(pByteData);
    configuration->attachment = r_cast<const void*>(ByteReader::readString(pByteData));
    return configuration;
  }

  void ConfigurationMapping::fromConfiguration(dxfg_configuration_t* configuration, ByteWriter& writer) {
    writer.writeString(configuration->event_symbol);
    writer.writeInt64_t(configuration->event_time);
    writer.writeInt32_t(configuration->version);
    writer.writeString(r_cast<const char*>(configuration->attachment));
  }
}