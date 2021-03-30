/* 
 * Reference Huffman coding
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/reference-huffman-coding
 * https://github.com/nayuki/Reference-Huffman-coding
 */

package br.ufsc.ine5431.huffmancoding;

/**
 * A leaf node in a code tree. It has a symbol value. Immutable.
 * @see CodeTree
 */
public final class Leaf extends Node {
	
	public final int symbol;  // Always non-negative
	
	
	
	public Leaf(int sym) {
		if (sym < 0)
			throw new IllegalArgumentException("Symbol value must be non-negative");
		symbol = sym;
	}
	
}
