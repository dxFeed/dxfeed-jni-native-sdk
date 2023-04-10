package com.dxfeed.api;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

class ByteBufferWrapper {
    private int pos = 0;
    private int totalBytes = 0;
    private final ByteBuffer[] byteChunks;
    private ConcurrentHashMap<String, byte[]> cacheStringToBytes;
    private byte[] data;

    ByteBufferWrapper(int quoteCount, ConcurrentHashMap<String, byte[]> cacheStringToBytes) {
        byteChunks = new ByteBuffer[quoteCount];
        this.cacheStringToBytes = cacheStringToBytes;
    }

    public void addChunk(int idx, int chunkSizeInBytes) {
        pos = 0;
        byteChunks[idx] = ByteBuffer.allocate(chunkSizeInBytes);
        data = byteChunks[idx].array();
        totalBytes += chunkSizeInBytes;
    }

    public void writeString(String value) {
        if (value == null || value.length() == 0) {
            writeInt(0); // 0 as empty str
        } else {
            writeInt(value.length());
            byte[] strBytes = cacheStringToBytes.computeIfAbsent(value, k -> value.getBytes(StandardCharsets.UTF_8));
            writeBytes(strBytes);
        }
    }

    public void writeByte(byte value) {
        data[pos++] = value;
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
        ByteBuffer res = ByteBuffer.allocate(totalBytes);
        for (ByteBuffer buffer : byteChunks) {
            res.put(buffer.array());
        }
        totalBytes = 0;
        data = null;
        return res.array();
    }
}