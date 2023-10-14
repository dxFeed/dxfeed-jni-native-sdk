package com.dxfeed.api.buffers;

import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

public class CString {
  private final static ConcurrentHashMap<String, byte[]> cacheStringToBytes = new ConcurrentHashMap<>();

  private final short cStringSize;
  public final short totalAllocatedBytes;
  public final byte[] strBytes;

  CString(String string) {
    cStringSize = calculateCStringSize(string);
    totalAllocatedBytes = (short) (cStringSize + 2); // 2 bytes for cStringSize value
    strBytes = (string != null) ? cacheStringToBytes.computeIfAbsent(string, k -> toCString(string)) : null;
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
//        System.out.println("string = " + value + ", result = " + Arrays.toString(result));
    return result;
  }
}
