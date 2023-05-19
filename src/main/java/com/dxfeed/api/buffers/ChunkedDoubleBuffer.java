package com.dxfeed.api.buffers;

public class ChunkedDoubleBuffer {
    private int pos = 0;
    private int totalSize = 0;
    private double[][] doubleChunks;
    private double[] data;

    public ChunkedDoubleBuffer(int quoteCount) {
        doubleChunks = new double[quoteCount][];
    }

    public void clear() {
        totalSize = pos = 0;
        data = null;
        doubleChunks = null;
    }

    public void addChunk(int idx, int chunkSize) {
        pos = 0;
        doubleChunks[idx] = new double[chunkSize];
        data = doubleChunks[idx];
        totalSize += chunkSize;
    }

    public void write(double v) {
        data[pos++] = v;
    }

    public double[] toDoubleData() {
        double[] result = new double[totalSize];
        int pos = 0;
        for (double[] buffer : doubleChunks) {
            System.arraycopy(buffer, 0, result, pos, buffer.length);
            pos += buffer.length;
        }
        totalSize = 0;
        data = null;
        return result;
    }
}