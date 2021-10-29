package br.ufsc.ine.leb.sistemaBancario;

public class TransacaoNaoRealizada implements Transacao {

	private Transacao transacao;

	public TransacaoNaoRealizada(Transacao transacao) {
		this.transacao = transacao;
	}

	@Override
	public ValorMonetario obterValorMonetario() {
		return transacao.obterValorMonetario();
	}

	@Override
	public ValorMonetario contabilizar(ValorMonetario saldo) {
		return saldo;
	}

}
