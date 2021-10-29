package br.ufsc.ine.leb.sistemaBancario;

import java.util.LinkedList;
import java.util.List;

public class Agencia {

	private Banco banco;
	private String nome;
	private Integer codigo;
	private List<Conta> contas;

	protected Agencia(String nome, Integer codigo, Banco banco) {
		this.nome = nome;
		this.codigo = codigo;
		this.banco = banco;
		this.contas = new LinkedList<>();
	}

	public Conta criarConta(String titular) {
		Conta conta = new Conta(titular, contas.size() + 1, this);
		contas.add(conta);
		return conta;
	}

	public String obterIdentificador() {
		return String.format("%03d", codigo);
	}

	public String obterNome() {
		return nome;
	}

	public Banco obterBanco() {
		return banco;
	}

}
