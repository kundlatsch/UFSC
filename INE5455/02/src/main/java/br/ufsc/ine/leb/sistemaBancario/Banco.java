package br.ufsc.ine.leb.sistemaBancario;

import java.util.LinkedList;
import java.util.List;

public class Banco {

	private Moeda moeda;
	private List<Agencia> agencias;
	private String nome;

	protected Banco(String nome, Moeda moeda, Dinheiro taxa) {
		this.nome = nome;
		this.moeda = moeda;
		this.agencias = new LinkedList<>();
	}

	public Agencia criarAgencia(String nome) {
		Agencia agencia = new Agencia(nome, agencias.size() + 1, this);
		agencias.add(agencia);
		return agencia;
	}
	
	public String obterNome() {
		return nome;
	}

	public Moeda obterMoeda() {
		return moeda;
	}

}
