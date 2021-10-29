package br.ufsc.ine.leb.sistemaBancario;

public class Dinheiro {

	private Moeda moeda;
	private Integer inteiro;
	private Integer fracionado;

	public Dinheiro(Moeda moeda, Integer inteiro, Integer fracionado) {
		this.moeda = moeda;
		this.inteiro = inteiro;
		this.fracionado = fracionado;
		normalizar();
	}

	private void normalizar() {
		Integer soma = obterQuantiaEmEscala();
		Integer baseFracionaria = moeda.obterBaseFracionaria();
		this.inteiro = (soma - (soma % baseFracionaria)) / baseFracionaria;
		this.fracionado = soma % baseFracionaria;
	}

	public Integer obterQuantiaEmEscala() {
		return Math.abs(inteiro) * moeda.obterBaseFracionaria() + Math.abs(fracionado);
	}

	public Moeda obterMoeda() {
		return moeda;
	}

	public String formatado() {
		return zero() ? formatarSemMoeda() : formatarComMoeda();
	}

	private String formatarSemMoeda() {
		return String.format("%d,%02d", inteiro, fracionado);
	}

	private String formatarComMoeda() {
		return String.format("%d,%02d %s", inteiro, fracionado, moeda.toString());
	}

	private Boolean zero() {
		return inteiro == 0 && fracionado == 0;
	}

	public ValorMonetario positivo() {
		return new ValorMonetario(moeda).somar(this);
	}

	public ValorMonetario negativo() {
		return new ValorMonetario(moeda).subtrair(this);
	}

	@Override
	public boolean equals(Object objeto) {
		if (objeto instanceof Dinheiro) {
			Dinheiro outro = (Dinheiro) objeto;
			Boolean mesmaMoeda = moeda.equals(outro.moeda);
			Boolean mesmoValor = inteiro.equals(outro.inteiro) && fracionado.equals(outro.fracionado);
			return zero() || (mesmoValor && mesmaMoeda);
		}
		return super.equals(objeto);
	}

	@Override
	public String toString() {
		return formatado();
	}

}
