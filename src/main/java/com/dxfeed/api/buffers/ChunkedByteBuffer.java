package com.dxfeed.api.buffers;

import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

public class ChunkedByteBuffer {
    private final static ConcurrentHashMap<String, byte[]> cacheStringToBytes = new ConcurrentHashMap<>();
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

    public void writeString(String value) {
        if (value == null || value.length() == 0) {
            writeShort((short)0); // 0 as empty str
        } else {
            writeShort((short)value.length());
            byte[] strBytes = cacheStringToBytes.computeIfAbsent(value, k -> value.getBytes(StandardCharsets.UTF_8));
            writeBytes(strBytes);
        }
    }

    public void writeByte(byte value) {
        data[pos++] = value;
    }

    public void writeShort(short value) {
        data[pos++] = (byte) value;
        data[pos++] = (byte) (value >> 8);
    }

    public void writeInt(int value) {
        data[pos++] = (byte) value;
        data[pos++] = (byte) (value >> 8);
        data[pos++] = (byte) (value >> 16);
        data[pos++] = (byte) (value >> 24);
    }

    public void writeLong(long value) {
        for (int i = 0; i < 8; i++) {
            data[pos++] = (byte) value;
            value = value >> 8;
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
        totalSize = 0;
        data = null;
        return result;
    }
}