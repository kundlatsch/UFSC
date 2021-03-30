package br.ufsc.ine5431.rlecoding;

import java.nio.ByteBuffer;

/* 
*  Implementa uma codificação RLE muito simplificada.
*  Realiza o deslocamento de 1 bit para a esquerda. O bit mais significativo
*  será o flag da repetição. Se o bit mais significativo estiver setado, os
*  sete bits seguintes indicam o número de repetições. Então no máximo temos
*  127 repetições que podem ser compactada. O byte seguinte ao número de 
*  repetições é o byte repetindo.
*/

public final class RLECoding {
	public static byte[]compress(byte[] input) {
		ByteBuffer bb = ByteBuffer.allocate(input.length);
		
		// Deslocamento a direita de 1 bit para colocar 0 no flag de repetição
		for (int i=0;i<input.length;i++)
			input[i] = (byte)(input[i]>>1&0x7f);

		int index=0; 
		byte flagnumber;
		while (index<input.length) {
			
			// identifica quantos bytes são repetidos 
			int repetition =1;
			for (int i=index+1;i<input.length;i++) {
				if (input[index]==input[i]) 
					repetition++;
				else
					break;
			}
			
			if (repetition==1) {
				// sem repetição
				bb.put(input[index++]);
			} else {
				// com repetição
				if (repetition<128) {
					// compacta
					flagnumber = (byte)(repetition|0x80);
					bb.put(flagnumber);
					bb.put(input[index]);
				} else {
					// repetição acima da capacidade de 7bit
					// faz divisão
					flagnumber = (byte)0xFF; // bit mais significativo setado e 127 repetições
					bb.put(flagnumber);
					bb.put(input[index]);
					int tmprepetition = repetition-127;
					while (tmprepetition!=0) {
						if (tmprepetition<=127) {
							flagnumber = (byte)(tmprepetition|0xF0);
							bb.put((byte)(tmprepetition|0x80));
							bb.put(input[index]);
							tmprepetition=0;
						} else {
							flagnumber = (byte)0xFF; // bit mais significativo setado e 127 repetições
							bb.put(flagnumber);
							bb.put(input[index]);
							tmprepetition -=127;
						}
					}
				}
				index +=repetition;
			}
		}
		if (index<input.length)
			bb.put(input[index]); // coloca o último byte sem repetição
		
		byte []bitstream = new byte[bb.position()];
		bb.rewind();
		bb.get(bitstream, 0, bitstream.length);
		return bitstream;
	}
	
	/* 
	 * Descompacta input RLE
	 */
	public static byte[]decompress(byte[] input) {
		ByteBuffer bb = ByteBuffer.allocate(input.length +256); // no mínimo 2 repetições de 127

		for (int i=0;i<input.length;i++) {
			if ((input[i]&0x80)==0) {
				// Próprio símbolo, não é repetição pois o flag mais significativo é "0"
				bb = putValue(bb,(byte)(input[i]<<1));
			} else {
				// Flag de repetição setado 
				int repetition = input[i]&0x7f; // zera flag de repetição
				for (int j=1;j<repetition;j++) {
					bb = putValue(bb,(byte)(input[i+1]<<1));
				}
			}
			// checha se o bb possui capacidade para mais 127 símbolos (pior caso próximo byte s ser analisado)
		}
		byte []bitstream = new byte[bb.position()];
		bb.rewind();
		bb.get(bitstream, 0, bitstream.length);
		return bitstream;
	}	
	
	private static ByteBuffer putValue(ByteBuffer bb, byte value) {
		if (bb.capacity()<=bb.position()+127) {
			// aumentar capacidade do ByteBuffer
			ByteBuffer bbtmp = ByteBuffer.allocate(bb.capacity()+1024);
			byte []data = new byte[bb.position()];
			bb.rewind();
			bb.get(data,0,data.length);
			bbtmp.put(data);
			bbtmp.put(value);
			return bbtmp;
		} else {
			bb.put(value);
			return bb;
		}
	}
}
