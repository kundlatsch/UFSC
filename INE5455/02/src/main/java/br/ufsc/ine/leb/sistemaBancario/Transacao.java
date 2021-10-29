package br.ufsc.ine.leb.sistemaBancario;

public interface Transacao {

	public ValorMonetario obterValorMonetario();

	public ValorMonetario contabilizar(ValorMonetario saldo);

}
