package br.ufsc.ine.leb.sistemaBancario;

public enum Moeda {

	BRL("R$", 100), USD("$", 100), CHF("Fr", 100);

	private String simbolo;
	private Integer baseFracionaria;

	private Moeda(String simbolo, Integer escala) {
		this.simbolo = simbolo;
		this.baseFracionaria = escala;
	}

	public String obterSimbolo() {
		return simbolo;
	}

	public Integer obterBaseFracionaria() {
		return baseFracionaria;
	}

}
