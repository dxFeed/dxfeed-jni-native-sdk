package com.dxfeed.api;

import java.nio.DoubleBuffer;

public class DoubleBufferWrapper {
    private final DoubleBuffer pDouble;
    private double[] data;

    private int doublePos = 0;

    DoubleBufferWrapper(int quoteCount, int doubleValuesPerQuote) {
        pDouble = DoubleBuffer.allocate(quoteCount * doubleValuesPerQuote);
        data = pDouble.array();
    }

    public void write(double v) {
        data[doublePos++] = v;
    }

    public double[] toDoubleData() {
        return data;
    }
}
