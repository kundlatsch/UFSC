package br.ufsc.ine.leb.sistemaBancario;

public class ValorMonetario {

	private Moeda moeda;
	private Integer sinal;
	private Dinheiro quantia;

	public ValorMonetario(Moeda moeda) {
		this(moeda, 0);
	}

	private ValorMonetario(Moeda moeda, Integer valor) {
		this.moeda = moeda;
		this.sinal = valor >= 0 ? 1 : -1;
		this.quantia = new Dinheiro(moeda, 0, valor);
	}

	public ValorMonetario somar(Dinheiro quantiaSomada) {
		validarMoeda(quantiaSomada);
		Integer valor = quantia.obterQuantiaEmEscala() * sinal + quantiaSomada.obterQuantiaEmEscala();
		return new ValorMonetario(moeda, valor);
	}

	public ValorMonetario subtrair(Dinheiro quantiaSubtraida) {
		validarMoeda(quantiaSubtraida);
		Integer valor = quantia.obterQuantiaEmEscala() * sinal - quantiaSubtraida.obterQuantiaEmEscala();
		return new ValorMonetario(moeda, valor);
	}

	public Dinheiro obterQuantia() {
		return quantia;
	}

	public Boolean negativo() {
		return sinal < 0;
	}

	public String formatado() {
		return zero() ? formatarSemSinal() : formatarComSinal();
	}

	public String formatarComSinal() {
		return negativo() ? formatarNegativo() : formatarPositivo();
	}

	public String formatarSemSinal() {
		return String.format("%s", quantia.formatado());
	}

	public Boolean zero() {
		return quantia.obterQuantiaEmEscala() == 0;
	}

	private String formatarPositivo() {
		return String.format("+%s", quantia.formatado());
	}

	private String formatarNegativo() {
		return String.format("-%s", quantia.formatado());
	}

	private void validarMoeda(Dinheiro quantiaSomada) {
		if (!moeda.equals(quantiaSomada.obterMoeda())) {
			throw new UnsupportedOperationException();
		}
	}

	@Override
	public boolean equals(Object objeto) {
		if (objeto instanceof ValorMonetario) {
			ValorMonetario outro = (ValorMonetario) objeto;
			Boolean iguaisZero = zero() && outro.zero();
			Boolean iguaisComMesmaMoeda = sinal.equals(outro.sinal) && quantia.equals(outro.quantia) && moeda.equals(outro.moeda);
			return iguaisZero || iguaisComMesmaMoeda;
		}
		return super.equals(objeto);
	}

	@Override
	public String toString() {
		return formatado();
	}

}
