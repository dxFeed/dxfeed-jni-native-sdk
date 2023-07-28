package com.dxfeed.api.buffers;

import java.util.Arrays;

public class ByteBuffer {
  private static final int EVENT_SIZE_IN_BYTES = 128;
  private byte[] data;
  private int totalSize;
  private int pos;

  public ByteBuffer(int eventCount) {
    totalSize = eventCount * EVENT_SIZE_IN_BYTES;
    data = new byte[totalSize];
    pos = 0;
  }

  public void clear() {
    pos = 0;
    totalSize = 0;
    data = null;
  }

  public void writeString(String str) {
    CString cString = new CString(str);
    writeShort(cString.cStringLength());
    if (cString.isNotNull()) {
      writeBytes(cString.strBytes);
    }
  }

  public void writeByte(byte value) {
    if (pos + Byte.SIZE >= totalSize) {
      resize();
    }
    data[pos++] = value;
  }

  public void writeShort(short value) {
    if (pos + Short.BYTES >= totalSize) {
      resize();
    }
    data[pos++] = (byte) (value & 0x00FF);
    data[pos++] = (byte) ((value & 0xFF00) >> Byte.SIZE);
  }

  public void writeChar(char value) {
    if (pos + Character.SIZE >= totalSize) {
      resize();
    }
    data[pos++] = (byte) (value & 0x00FF);
    data[pos++] = (byte) ((value & 0xFF00) >> Byte.SIZE);
  }

  public void writeInt(int value) {
    if (pos + Integer.SIZE >= totalSize) {
      resize();
    }
    for (int i = 0; i < Integer.BYTES; i++) {
      data[pos++] = (byte) (value & 0xFF);
      value >>= Byte.SIZE;
    }
  }

  public void writeLong(long value) {
    if (pos + Long.SIZE >= totalSize) {
      resize();
    }
    for (int i = 0; i < Long.BYTES; i++) {
      data[pos++] = (byte) (value & 0xFF);
      value >>= Byte.SIZE;
    }
  }

  public void writeBytes(byte[] value) {
    if (pos + value.length >= totalSize) {
      resize();
    }
    System.arraycopy(value, 0, data, pos, value.length);
    pos += value.length;
  }

  public byte[] toByteData() {
    return data;
  }

  private void resize() {
    totalSize *= 2;
    data = Arrays.copyOf(data, totalSize);
  }
}