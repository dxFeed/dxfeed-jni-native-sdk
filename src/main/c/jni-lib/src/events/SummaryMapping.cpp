// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/SummaryMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_summary_t* SummaryMapping::toSummary(const char** pByteData, const double** pDoubleData) {
    auto summary = new dxfg_summary_t();
    summary->market_event.event_type.clazz = DXFG_EVENT_SUMMARY;
    summary->market_event.event_symbol = EventReader::readString(pByteData);
    summary->market_event.event_time = EventReader::readLong(pByteData);
    summary->day_id = EventReader::readInt(pByteData);
    summary->prev_day_id = EventReader::readInt(pByteData);
    summary->open_interest = EventReader::readLong(pByteData);
    summary->flags = EventReader::readInt(pByteData);

    summary->day_open_price = EventReader::readDouble(pDoubleData);
    summary->day_high_price = EventReader::readDouble(pDoubleData);
    summary->day_low_price = EventReader::readDouble(pDoubleData);
    summary->day_close_price = EventReader::readDouble(pDoubleData);
    summary->prev_day_close_price = EventReader::readDouble(pDoubleData);
    summary->prev_day_volume = EventReader::readDouble(pDoubleData);

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