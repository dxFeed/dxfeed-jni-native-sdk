package com.dxfeed.api;

import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

class ByteBufferWrapper {
    private int pos = 0;
    private int totalBytes = 0;
    private final byte[][] byteChunks;
    private final ConcurrentHashMap<String, byte[]> cacheStringToBytes;
    private byte[] data;

    ByteBufferWrapper(int quoteCount, ConcurrentHashMap<String, byte[]> cacheStringToBytes) {
        byteChunks = new byte[quoteCount][];
        this.cacheStringToBytes = cacheStringToBytes;
    }

    public void addChunk(int idx, int chunkSizeInBytes) {
        pos = 0;
        byteChunks[idx] = new byte[chunkSizeInBytes];
        data = byteChunks[idx];
        totalBytes += chunkSizeInBytes;
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
        byte[] result = new byte[totalBytes];
        int pos = 0;
        for (byte[] buffer : byteChunks) {
            System.arraycopy(buffer, 0, result, pos, buffer.length);
            pos += buffer.length;
        }
        totalBytes = 0;
        data = null;
        return result;
    }
}