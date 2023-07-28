package com.dxfeed.api.buffers;

import java.util.Arrays;

public class DoubleBuffer {
  private static final int EVENT_DATA_IN_DOUBLES = 16;
  private double[] data;
  private int totalSize;
  private int pos;

  public DoubleBuffer(int eventCount) {
    totalSize = eventCount * EVENT_DATA_IN_DOUBLES;
    data = new double[totalSize];
    pos = 0;
  }

  public void clear() {
    pos = 0;
    totalSize = 0;
    data = null;
  }

  public void write(double v) {
    if (pos == totalSize - 1) {
      resize();
    }
    data[pos++] = v;
  }

  public double[] toDoubleData() {
    return data;
  }

  private void resize() {
    totalSize *= 2;
    data = Arrays.copyOf(data, totalSize);
  }
}