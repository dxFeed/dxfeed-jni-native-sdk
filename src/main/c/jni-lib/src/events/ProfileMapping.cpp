// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/ProfileMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_profile_t* ProfileMapping::toProfile(const char** pByteData, const double** pDoubleData) {
    auto* profile = new dxfg_profile_t();
    profile->market_event.event_type.clazz = DXFG_EVENT_PROFILE;
    profile->market_event.event_symbol = ByteReader::readString(pByteData);
    profile->market_event.event_time = ByteReader::readLong(pByteData);
    profile->halt_start_time = ByteReader::readLong(pByteData);
    profile->halt_end_time = ByteReader::readLong(pByteData);
    profile->ex_dividend_day_id = ByteReader::readInt(pByteData);
    profile->flags = ByteReader::readInt(pByteData);
    profile->description = ByteReader::readString(pByteData);
    profile->status_reason = ByteReader::readString(pByteData);

    profile->high_limit_price = ByteReader::readDouble(pDoubleData);
    profile->low_limit_price = ByteReader::readDouble(pDoubleData);
    profile->high_52_week_price = ByteReader::readDouble(pDoubleData);
    profile->low_52_week_price = ByteReader::readDouble(pDoubleData);
    profile->beta = ByteReader::readDouble(pDoubleData);
    profile->earnings_per_share = ByteReader::readDouble(pDoubleData);
    profile->dividend_frequency = ByteReader::readDouble(pDoubleData);
    profile->ex_dividend_amount = ByteReader::readDouble(pDoubleData);
    profile->shares = ByteReader::readDouble(pDoubleData);
    profile->free_float = ByteReader::readDouble(pDoubleData);
    return profile;
  }

  void ProfileMapping::fromProfile(dxfg_profile_t* profile, ByteWriter& writer) {
    writer.writeString(profile->market_event.event_symbol);
    writer.writeInt64_t(profile->market_event.event_time);
    writer.writeInt64_t(profile->halt_start_time);
    writer.writeInt64_t(profile->halt_end_time);
    writer.writeInt32_t(profile->ex_dividend_day_id);
    writer.writeInt32_t(profile->flags);
    writer.writeString(profile->description);
    writer.writeString(profile->status_reason);

    writer.writeDouble(profile->high_limit_price);
    writer.writeDouble(profile->low_limit_price);
    writer.writeDouble(profile->high_52_week_price);
    writer.writeDouble(profile->low_52_week_price);
    writer.writeDouble(profile->beta);
    writer.writeDouble(profile->earnings_per_share);
    writer.writeDouble(profile->dividend_frequency);
    writer.writeDouble(profile->ex_dividend_amount);
    writer.writeDouble(profile->shares);
    writer.writeDouble(profile->free_float);
  }
}