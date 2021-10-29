package br.ufsc.ine.leb.sistemaBancario;

import java.util.LinkedList;
import java.util.List;

public class Conta {

	private String titular;
	private Integer codigo;
	private Agencia agencia;
	private List<Transacao> transacoes;

	protected Conta(String titular, Integer codigo, Agencia agencia) {
		this.titular = titular;
		this.codigo = codigo;
		this.agencia = agencia;
		this.transacoes = new LinkedList<>();
	}

	public String obterIdentificador() {
		return String.format("%04d-%d", codigo, titular.length() % 10);
	}

	public String obterTitular() {
		return titular;
	}

	public Agencia obterAgencia() {
		return agencia;
	}

	public ValorMonetario calcularSaldo() {
		Moeda moeda = obterAgencia().obterBanco().obterMoeda();
		ValorMonetario saldo = new Dinheiro(moeda, 0, 0).positivo();
		for (Transacao transacao : transacoes) {
			saldo = transacao.contabilizar(saldo);
		}
		return saldo;
	}

	public void adicionarTransacao(Transacao transacao) {
		transacoes.add(transacao);
	}

}
