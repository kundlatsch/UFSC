package br.ufsc.ine5431.huffmancoding;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

class ByteBufferBackedInputStream extends InputStream{
  
  ByteBuffer buf;
  ByteBufferBackedInputStream( ByteBuffer buf){
    this.buf = buf;
  }
  public synchronized int read() throws IOException {
    if (!buf.hasRemaining()) {
      return -1;
    }
    return buf.get();
  }
  public synchronized int read(byte[] bytes, int off, int len) throws IOException {
    len = Math.min(len, buf.remaining());
    buf.get(bytes, off, len);
    return len;
  }
}