package com.dxfeed.api;

public class DoubleBufferWrapper {
    private final double[] data;
    private int doublePos = 0;

    DoubleBufferWrapper(int quoteCount, int doubleValuesPerQuote) {
        data = new double[quoteCount * doubleValuesPerQuote];
    }

    public void write(double v) {
        data[doublePos++] = v;
    }

    public double[] toDoubleData() {
        return data;
    }
}
