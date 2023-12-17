// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "dxfeed/events/SeriesMapping.hpp"
#include "dxfeed/utils/ByteReader.hpp"
#include "dxfeed/utils/ByteWriter.hpp"

namespace dxfeed::jni {
  dxfg_series_t* SeriesMapping::toSeries(ByteReader& reader) {
    auto* series = new dxfg_series_t();
    series->market_event.event_type.clazz = DXFG_EVENT_SERIES;
    series->market_event.event_symbol = reader.readString();
    series->market_event.event_time = reader.readLong();
    series->event_flags = reader.readInt();
    series->index = reader.readLong();
    series->time_sequence = reader.readLong();
    series->expiration = reader.readInt();

    series->volatility = reader.readDouble();
    series->call_volume = reader.readDouble();
    series->put_volume = reader.readDouble();
    series->put_call_ratio = reader.readDouble();
    series->forward_price = reader.readDouble();
    series->dividend = reader.readDouble();
    series->interest = reader.readDouble();
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