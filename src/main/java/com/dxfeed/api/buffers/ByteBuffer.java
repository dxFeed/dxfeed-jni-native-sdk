// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

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
    CString cString = CString.build(str);
    writeShort(cString.cStringLength());
    if (cString.isNotNull()) {
      writeBytes(cString.strBytes);
    }
  }

  public void writeByte(byte value) {
    if (pos + Byte.BYTES >= totalSize) {
      resize();
    }
    data[pos++] = value;
  }

  public void writeShort(short value) {
    if (pos + Short.BYTES >= totalSize) {
      resize();
    }
    data[pos++] = (byte) ((value >> Byte.SIZE) & 0xFF);
    data[pos++] = (byte) (value & 0xFF);
  }

  public void writeChar(char value) {
    if (pos + Character.BYTES >= totalSize) {
      resize();
    }
    data[pos++] = (byte) ((value >> Byte.SIZE) & 0xFF);
    data[pos++] = (byte) (value & 0xFF);
  }

  public void writeInt(int value) {
    if (pos + Integer.BYTES >= totalSize) {
      resize();
    }
    for (int i = Integer.BYTES - 1; i >= 0; --i) {
      data[pos + i] = (byte) (value & 0xFF);
      value >>= Byte.SIZE;
    }
    pos += Integer.BYTES;
  }

  public void writeLong(long value) {
    if (pos + Long.BYTES >= totalSize) {
      resize();
    }
    for (int i = Long.BYTES - 1; i >= 0; --i) {
      data[pos + i] = (byte) (value & 0xFF);
      value >>= Byte.SIZE;
    }
    pos += Long.BYTES;
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