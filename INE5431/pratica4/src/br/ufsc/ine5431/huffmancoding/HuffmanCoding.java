/* 
 * Reference Huffman coding
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/reference-huffman-coding
 * https://github.com/nayuki/Reference-Huffman-coding
 */

package br.ufsc.ine5431.huffmancoding;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

/**
 * Compression application using static Huffman coding.
 * <p>Usage: java HuffmanCompress InputFile OutputFile</p>
 * <p>Then use the corresponding "HuffmanDecompress" application to recreate the original input file.</p>
 * <p>Note that the application uses an alphabet of 257 symbols - 256 symbols for the byte values
 * and 1 symbol for the EOF marker. The compressed file format starts with a list of 257
 * code lengths, treated as a canonical code, and then followed by the Huffman-coded data.</p>
 */
public final class HuffmanCoding {
	
	// Command line main application function.
		public static byte[] decompression(byte[] compresseddata) throws IOException {
			
	   	 	BitInputStream in = new BitInputStream(new ByteArrayInputStream(compresseddata));
	   	 	
	   	 	int maxsize = compresseddata.length*3;
	   	 	if (maxsize<20000)
	   	 		maxsize=20000;
	   	 	
			ByteBuffer buf = ByteBuffer.allocate(maxsize); // prevendo taxa de compressão de 3x.
			OutputStream out = new ByteBufferBackedOutputStream(buf);


			// Perform file decompression

			try {
				CanonicalCode canonCode = readCodeLengthTable(in);
				CodeTree code = canonCode.toCodeTree();
				decompress(code, in, out);
				out.close();
		   	 	byte[] data = new byte[buf.position()];
		   	 	buf.rewind();
		   	 	buf.get(data);
			    return data;
			} finally {
				in.close();
			}
		}
	// Command line main application function.
	public static byte[] compression (byte []source) throws IOException {
   	 	
   	 	InputStream in = new ByteArrayInputStream(source);
		FrequencyTable freqs = getFrequencies(in);
		freqs.increment(256);  // EOF symbol gets a frequency of 1
		CodeTree code = freqs.buildCodeTree();
		System.out.println("Tabela de codificação de Huffman");
		System.out.println(code.toString());
		CanonicalCode canonCode = new CanonicalCode(code, 257);
		// Replace code tree with canonical one. For each symbol,
		// the code value may change but the code length stays the same.
		code = canonCode.toCodeTree();
		
		// Read input  again, compress with Huffman coding, and write output 

   	 	int maxsize = source.length;
   	 	if (maxsize<20000)
   	 		maxsize=20000;

		ByteBuffer buf = ByteBuffer.allocate(maxsize);
		OutputStream os = new ByteBufferBackedOutputStream(buf);

		BitOutputStream out = new BitOutputStream(os);

		
		try {
			writeCodeLengthTable(out, canonCode);
			compress(code, in, out);
			out.close();
	   	 	byte[] data = new byte[buf.position()];
	   	 	buf.rewind();
	   	 	buf.get(data);
		    return data;
		} finally {
			in.close();
		}
	}
	
	// To allow unit testing, this method is package-private instead of private.
	static CanonicalCode readCodeLengthTable(BitInputStream in) throws IOException {
		int[] codeLengths = new int[257];
		for (int i = 0; i < codeLengths.length; i++) {
			// For this file format, we read 8 bits in big endian
			int val = 0;
			for (int j = 0; j < 8; j++)
				val = (val << 1) | in.readNoEof();
			codeLengths[i] = val;
		}
		return new CanonicalCode(codeLengths);
	}
	
	// To allow unit testing, this method is package-private instead of private.
		static void decompress(CodeTree code, BitInputStream in, OutputStream out) throws IOException {
			HuffmanDecoder dec = new HuffmanDecoder(in);
			dec.codeTree = code;
			int symbol;
			while (true) {
				symbol = dec.read();
				if (symbol == 256)  // EOF symbol
					break;
				out.write(symbol);
			}
		}
	
	// Returns a frequency table based on the bytes in the given InputStream.
	// Also contains an extra entry for symbol 256, whose frequency is set to 0.
	private static FrequencyTable getFrequencies(InputStream input) throws IOException {
		FrequencyTable freqs = new FrequencyTable(new int[257]);
		try {
			while (true) {
				int b = input.read();
				if (b == -1)
					break;
				freqs.increment(b);
			}
		} finally {
			input.close();
		}
		return freqs;
	}
	
	
	// To allow unit testing, this method is package-private instead of private.
	static void writeCodeLengthTable(BitOutputStream out, CanonicalCode canonCode) throws IOException {
		for (int i = 0; i < canonCode.getSymbolLimit(); i++) {
			int val = canonCode.getCodeLength(i);
			// For this file format, we only support codes up to 255 bits long
			if (val >= 256)
				throw new RuntimeException("The code for a symbol is too long");
			
			// Write value as 8 bits in big endian
			for (int j = 7; j >= 0; j--) {
				out.write((val >>> j) & 1);
			}
		}
	}
	
	
	// To allow unit testing, this method is package-private instead of private.
	static void compress(CodeTree code, InputStream in, BitOutputStream out) throws IOException {
		in.reset();
		HuffmanEncoder enc = new HuffmanEncoder(out);
		enc.codeTree = code;
		while (true) {
			int b = in.read();
			if (b == -1)
				break;
			enc.write(b);
		}
		enc.write(256);  // EOF
	}	
}
