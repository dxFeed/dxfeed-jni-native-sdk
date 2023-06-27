package com.dxfeed.api.buffers;

import com.dxfeed.api.serializers.CString;

public class ChunkedByteBuffer {
  private int pos = 0;
  private int totalSize = 0;
  private byte[][] byteChunks;
  private byte[] data;

  public ChunkedByteBuffer(int quoteCount) {
    byteChunks = new byte[quoteCount][];
  }

  public void clear() {
    totalSize = pos = 0;
    data = null;
    byteChunks = null;
  }

  public void addChunk(int idx, int chunkSizeInBytes) {
    pos = 0;
    byteChunks[idx] = new byte[chunkSizeInBytes];
    data = byteChunks[idx];
    totalSize += chunkSizeInBytes;
  }

  public void writeString(CString cString) {
    writeShort(cString.cStringLength());
    if (cString.isNotNull()) {
      writeBytes(cString.strBytes);
    }
  }

  public void writeByte(byte value) {
    data[pos++] = value;
  }

  public void writeShort(short value) {
    data[pos++] = (byte) (value & 0x00FF);
    data[pos++] = (byte) ((value & 0xFF00) >> Byte.SIZE);
  }

  public void writeChar(char value) {
    data[pos++] = (byte) (value & 0x00FF);
    data[pos++] = (byte) ((value & 0xFF00) >> Byte.SIZE);
  }

  public void writeInt(int value) {
    for (int i = 0; i < Integer.BYTES; i++) {
      data[pos++] = (byte) (value & 0xFF);
      value >>= Byte.SIZE;
    }
  }

  public void writeLong(long value) {
    for (int i = 0; i < Long.BYTES; i++) {
      data[pos++] = (byte) (value & 0xFF);
      value >>= Byte.SIZE;
    }
  }

  public void writeBytes(byte[] value) {
    System.arraycopy(value, 0, data, pos, value.length);
    pos += value.length;
  }

  public byte[] toByteData() {
    byte[] result = new byte[totalSize];
    int pos = 0;
    for (byte[] buffer : byteChunks) {
      System.arraycopy(buffer, 0, result, pos, buffer.length);
      pos += buffer.length;
    }
    return result;
  }
}