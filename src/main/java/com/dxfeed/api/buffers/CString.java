// Copyright © 2023 Devexperts LLC. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
// If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

package com.dxfeed.api.buffers;

import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

public class CString {
  private static final CString DUMMY = new CString(null);
  private final static ConcurrentHashMap<String, CString> cacheStringToBytes = new ConcurrentHashMap<>();

  private final short cStringSize;
  public final short totalAllocatedBytes;
  public final byte[] strBytes;

  public static CString build(String string) {
    return (string != null) ? cacheStringToBytes.computeIfAbsent(string, k -> new CString(string)) : DUMMY;
  }
  private CString(String string) {
    cStringSize = calculateCStringSize(string);
    totalAllocatedBytes = (short) (cStringSize + 2); // 2 bytes for cStringSize value
    strBytes = (string != null) ? toCString(string) : null;
  }

  private static short calculateCStringSize(String s) {
    return (short) ((s != null) ? s.length() + 1 : 0);
  }

  public boolean isNotNull() {
    return cStringSize != 0;
  }

  public short cStringLength() {
    return cStringSize;
  }

  public static byte[] toCString(String value) {
    byte[] bytes = value.getBytes(StandardCharsets.UTF_8);
    byte[] result = new byte[bytes.length + 1];
    result[bytes.length] = 0;
    System.arraycopy(bytes, 0, result, 0, bytes.length);
    return result;
  }
}
