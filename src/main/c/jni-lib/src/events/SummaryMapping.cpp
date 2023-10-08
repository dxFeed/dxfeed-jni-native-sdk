// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/SummaryMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_summary_t* SummaryMapping::toSummary(ByteReader& reader) {
    auto summary = new dxfg_summary_t();
    summary->market_event.event_type.clazz = DXFG_EVENT_SUMMARY;
    summary->market_event.event_symbol = reader.readString();
    summary->market_event.event_time = reader.readLong();
    summary->day_id = reader.readInt();
    summary->prev_day_id = reader.readInt();
    summary->open_interest = reader.readLong();
    summary->flags = reader.readInt();

    summary->day_open_price = reader.readDouble();
    summary->day_high_price = reader.readDouble();
    summary->day_low_price = reader.readDouble();
    summary->day_close_price = reader.readDouble();
    summary->prev_day_close_price = reader.readDouble();
    summary->prev_day_volume = reader.readDouble();
    return summary;
  }

  void SummaryMapping::fromSummary(dxfg_summary_t* summary, ByteWriter& writer) {
    writer.writeString(summary->market_event.event_symbol);
    writer.writeInt64_t(summary->market_event.event_time);
    writer.writeInt32_t(summary->day_id);
    writer.writeInt32_t(summary->prev_day_id);
    writer.writeInt64_t(summary->open_interest);
    writer.writeInt32_t(summary->flags);

    writer.writeDouble(summary->day_open_price);
    writer.writeDouble(summary->day_high_price);
    writer.writeDouble(summary->day_low_price);
    writer.writeDouble(summary->day_close_price);
    writer.writeDouble(summary->prev_day_close_price);
    writer.writeDouble(summary->prev_day_volume);
  }
}