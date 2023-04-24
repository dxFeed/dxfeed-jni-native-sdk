package com.dxfeed.api.serializers;

import com.dxfeed.api.buffers.ChunkedByteBuffer;
import com.dxfeed.api.buffers.ChunkedDoubleBuffer;
import com.dxfeed.event.market.Quote;

public class QuoteToNative {
    public static void convert(Quote quote, ChunkedByteBuffer pBytes, ChunkedDoubleBuffer pDoubles, int eventIdx) {

        int totalSize = 0;

        pBytes.addChunk(eventIdx, totalSize);

        // DOUBLE DATA
        pBytes.addChunk(eventIdx, 3);

    }
}
