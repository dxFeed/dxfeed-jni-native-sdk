// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/SeriesMapping.h"
#include "dxfeed/events/EventReader.h"

namespace dxfeed {

  dxfg_series_t* SeriesMapping::toSeries(const char** pByteData, const double** pDoubleData) {
    auto* series = new dxfg_series_t();
    series->market_event.event_type.clazz = DXFG_EVENT_SERIES;
    series->market_event.event_symbol = EventReader::readString(pByteData);
    series->market_event.event_time = EventReader::readLong(pByteData);
    series->event_flags = EventReader::readInt(pByteData);
    series->index = EventReader::readLong(pByteData);
    series->time_sequence = EventReader::readLong(pByteData);
    series->expiration = EventReader::readInt(pByteData);

    series->volatility = EventReader::readDouble(pDoubleData);
    series->call_volume = EventReader::readDouble(pDoubleData);
    series->put_volume = EventReader::readDouble(pDoubleData);
    series->put_call_ratio = EventReader::readDouble(pDoubleData);
    series->forward_price = EventReader::readDouble(pDoubleData);
    series->dividend = EventReader::readDouble(pDoubleData);
    series->interest = EventReader::readDouble(pDoubleData);
    return series;
  }

  void SeriesMapping::fromSeries(dxfg_series_t* series, ByteWriter& writer) {
    writer.writeString(series->market_event.event_symbol);
    writer.writeInt64_t(series->market_event.event_time);
    writer.writeInt32_t(series->event_flags);
    writer.writeInt64_t(series->index);
    writer.writeInt64_t(series->time_sequence);
    writer.writeInt32_t(series->expiration);

    writer.writeDouble(series->volatility);
    writer.writeDouble(series->call_volume);
    writer.writeDouble(series->put_volume);
    writer.writeDouble(series->put_call_ratio);
    writer.writeDouble(series->forward_price);
    writer.writeDouble(series->dividend);
    writer.writeDouble(series->interest);
  }
}