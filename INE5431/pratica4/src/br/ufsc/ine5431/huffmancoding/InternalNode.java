package br.ufsc.ine5431.huffmancoding;

import java.util.Objects;

/* 
 * Reference Huffman coding
 * Copyright (c) Project Nayuki
 * 
 * https://www.nayuki.io/page/reference-huffman-coding
 * https://github.com/nayuki/Reference-Huffman-coding
 */


/**
 * An internal node in a code tree. It has two nodes as children. Immutable.
 * @see CodeTree
 */
public final class InternalNode extends Node {
	
	public final Node leftChild;  // Not null
	
	public final Node rightChild;  // Not null
	
	
	
	public InternalNode(Node left, Node right) {
		Objects.requireNonNull(left);
		Objects.requireNonNull(right);
		leftChild = left;
		rightChild = right;
	}
	
}
