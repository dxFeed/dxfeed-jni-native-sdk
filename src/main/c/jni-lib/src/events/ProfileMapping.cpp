// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/events/ProfileMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_profile_t* ProfileMapping::toProfile(ByteReader& reader) {
    auto* profile = new dxfg_profile_t();
    profile->market_event.event_type.clazz = DXFG_EVENT_PROFILE;
    profile->market_event.event_symbol = reader.readString();
    profile->market_event.event_time = reader.readLong();
    profile->halt_start_time = reader.readLong();
    profile->halt_end_time = reader.readLong();
    profile->ex_dividend_day_id = reader.readInt();
    profile->flags = reader.readInt();
    profile->description = reader.readString();
    profile->status_reason = reader.readString();

    profile->high_limit_price = reader.readDouble();
    profile->low_limit_price = reader.readDouble();
    profile->high_52_week_price = reader.readDouble();
    profile->low_52_week_price = reader.readDouble();
    profile->beta = reader.readDouble();
    profile->earnings_per_share = reader.readDouble();
    profile->dividend_frequency = reader.readDouble();
    profile->ex_dividend_amount = reader.readDouble();
    profile->shares = reader.readDouble();
    profile->free_float = reader.readDouble();
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