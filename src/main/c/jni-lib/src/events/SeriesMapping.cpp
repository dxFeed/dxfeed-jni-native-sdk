// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/events/SeriesMapping.h"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_series_t* SeriesMapping::toSeries(const char** pByteData, const double** pDoubleData) {
    auto* series = new dxfg_series_t();
    series->market_event.event_type.clazz = DXFG_EVENT_SERIES;
    series->market_event.event_symbol = ByteReader::readString(pByteData);
    series->market_event.event_time = ByteReader::readLong(pByteData);
    series->event_flags = ByteReader::readInt(pByteData);
    series->index = ByteReader::readLong(pByteData);
    series->time_sequence = ByteReader::readLong(pByteData);
    series->expiration = ByteReader::readInt(pByteData);

    series->volatility = ByteReader::readDouble(pDoubleData);
    series->call_volume = ByteReader::readDouble(pDoubleData);
    series->put_volume = ByteReader::readDouble(pDoubleData);
    series->put_call_ratio = ByteReader::readDouble(pDoubleData);
    series->forward_price = ByteReader::readDouble(pDoubleData);
    series->dividend = ByteReader::readDouble(pDoubleData);
    series->interest = ByteReader::readDouble(pDoubleData);
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