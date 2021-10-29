package br.ufsc.ine.leb.sistemaBancario;

import br.ufsc.ine.leb.sistemaBancario.*;

public class AuxiliarDeTestes {

    private SistemaBancario sistemaBancario;
	private Banco bancoDoBrasil;
    private Agencia agenciaCentro;
    private Conta contaMaria;

    // Delegated Setup
    public AuxiliarDeTestes(SistemaBancario sistemaBancario) {
        this.sistemaBancario = sistemaBancario;
        bancoDoBrasil = sistemaBancario.criarBanco("Banco do Brasil", Moeda.BRL);
        agenciaCentro = bancoDoBrasil.criarAgencia("Centro");
        contaMaria = agenciaCentro.criarConta("Maria");
    }

	public Conta obterContaMaria() {
		return contaMaria;
	}

}