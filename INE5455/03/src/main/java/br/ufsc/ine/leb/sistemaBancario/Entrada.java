package br.ufsc.ine.leb.sistemaBancario;

public class Entrada implements Transacao {

	private Dinheiro quantia;

	public Entrada(Conta conta, Dinheiro quantia) {
		this.quantia = quantia;
	}

	@Override
	public ValorMonetario obterValorMonetario() {
		return quantia.positivo();
	}

	@Override
	public ValorMonetario contabilizar(ValorMonetario saldo) {
		return saldo.somar(quantia);
	}

}
