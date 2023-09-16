// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/ProfileMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_profile_t* ProfileMapping::toProfile(const char** pByteData, const double** pDoubleData) {
    auto* profile = new dxfg_profile_t();
    profile->market_event.event_type.clazz = DXFG_EVENT_PROFILE;
    profile->market_event.event_symbol = EventReader::readString(pByteData);
    profile->market_event.event_time = EventReader::readLong(pByteData);
    profile->halt_start_time = EventReader::readLong(pByteData);
    profile->halt_end_time = EventReader::readLong(pByteData);
    profile->ex_dividend_day_id = EventReader::readInt(pByteData);
    profile->flags = EventReader::readInt(pByteData);
    profile->description = EventReader::readString(pByteData);
    profile->status_reason = EventReader::readString(pByteData);

    profile->high_limit_price = EventReader::readDouble(pDoubleData);
    profile->low_limit_price = EventReader::readDouble(pDoubleData);
    profile->high_52_week_price = EventReader::readDouble(pDoubleData);
    profile->low_52_week_price = EventReader::readDouble(pDoubleData);
    profile->beta = EventReader::readDouble(pDoubleData);
    profile->earnings_per_share = EventReader::readDouble(pDoubleData);
    profile->dividend_frequency = EventReader::readDouble(pDoubleData);
    profile->ex_dividend_amount = EventReader::readDouble(pDoubleData);
    profile->shares = EventReader::readDouble(pDoubleData);
    profile->free_float = EventReader::readDouble(pDoubleData);
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