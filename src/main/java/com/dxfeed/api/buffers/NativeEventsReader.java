package com.dxfeed.api.buffers;


import java.nio.charset.StandardCharsets;

public class NativeEventsReader {
  private final byte[] byteData;
  private final double[] doubleData;
  private int bytePos = 0;
  private int doublePos = 0;

  public NativeEventsReader(byte[] byteData, double[] doubleData) {
    this.byteData = byteData;
    this.doubleData = doubleData;
  }

  private byte readByte() {
    return byteData[bytePos++];
  }

  public String readString() {
    char size = readChar();
    String str = new String(byteData, bytePos, size, StandardCharsets.UTF_8);
    bytePos += size;
    return str;
  }

  public short readShort() {
    short value = 0;
    for (int i = 0; i < Short.BYTES; ++i) {
      value |= readByte() << (i * 8);
    }
    return value;
  }

  public char readChar() {
    char value = 0;
    for (int i = 0; i < Character.BYTES; ++i) {
      value |= readByte() << (i * 8);
    }
    return value;
  }

  public int readInt() {
    int value = 0;
    for (int i = 0; i < Integer.BYTES; ++i) {
      value |= readByte() << (i * 8);
    }
    return value;
  }

  public long readLong() {
    long value = 0;
    for (int i = 0; i < Long.BYTES; ++i) {
      value |= (long)(readByte()) << (i * 8);
    }
    return value;
  }

  public double readDouble() {
    return doubleData[doublePos++];
  }
}
