package br.ufsc.ine.leb.sistemaBancario;

import br.ufsc.ine.leb.sistemaBancario.*;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.hamcrest.MatcherAssert.assertThat; 
import static org.hamcrest.Matchers.*;

import java.util.List;

public class Testes {

	private SistemaBancario sistemaBancario;
	
	// Implicit Setup
	@BeforeEach
	void setup() {
		sistemaBancario = new SistemaBancario();
	}

	private void assertBanco(Banco banco, String nome, String simboloMoeda, Integer escalaMoeda) {
		assertThat(banco.obterNome(), anyOf(
			equalToIgnoringCase(nome), 
			equalToIgnoringWhiteSpace(nome)
		));
		assertThat(banco.obterMoeda().obterSimbolo(), equalToIgnoringCase(simboloMoeda));
		assertThat(banco.obterMoeda().obterBaseFracionaria(), allOf(
			comparesEqualTo(escalaMoeda),
			greaterThan(0)
		));
	}

	private void assertAgencia(Agencia agencia, String nome, String identificador, Banco banco) {
		assertThat(agencia.obterNome(), anyOf(
			equalToIgnoringCase(nome),
			equalToIgnoringWhiteSpace(nome)
		));
		assertThat(agencia.obterIdentificador(), equalToIgnoringWhiteSpace(identificador));
		assertThat(agencia.obterBanco(), equalTo(banco));

		List<Banco> bancos = sistemaBancario.obterBancos();
		assertThat(bancos, hasItem(banco));
	}

	private void assertConta(Conta conta, String titular, String identificador, String saldo, Agencia agencia) {
		assertThat(conta.obterTitular(), anyOf(
			equalToIgnoringCase(titular),
			equalToIgnoringWhiteSpace(titular)
		));
		assertThat(conta.obterIdentificador(), equalToIgnoringWhiteSpace(identificador));
		assertThat(conta.calcularSaldo().toString(), equalToIgnoringWhiteSpace(saldo));
		assertThat(conta.obterAgencia(), equalTo(agencia));
	}

	private void assertOperacao(Conta conta, String saldoEsperado, Operacao operacao, EstadosDeOperacao estado) {
		assertThat(conta.calcularSaldo().toString(), equalToIgnoringWhiteSpace(saldoEsperado));
		assertThat(operacao.obterEstado(), is(estado));
	}

	@Test
	void testarBanco() {
		//Fixture Setup
		
		//Exercise SUT
		Banco bancoDoBrasil = sistemaBancario.criarBanco("Banco do Brasil", Moeda.BRL);
		
		//Result Verification
		assertBanco(bancoDoBrasil, "banco do brasil", "R$", 100);
		
		//Fixture Teardown
	}

	@Test
	void testarAgencia() {
		//Fixture Setup
		Banco bancoDoBrasil = sistemaBancario.criarBanco("Banco do Brasil", Moeda.BRL);
		
		//Exercise SUT
		Agencia agenciaCentro = bancoDoBrasil.criarAgencia("Centro");
		
		//Result Verification
		assertAgencia(agenciaCentro, "centro", "001", bancoDoBrasil);
		
		//Fixture Teardown
	}

	@Test
	void testarConta() {
		//Fixture Setup
		Banco bancoDoBrasil = sistemaBancario.criarBanco("Banco do Brasil", Moeda.BRL);
		Agencia agenciaCentro = bancoDoBrasil.criarAgencia("Centro");
		
		//Exercise SUT
		Conta contaMaria = agenciaCentro.criarConta("Maria");

		//Result Verification
		assertConta(contaMaria, "maria", "0001-5", "0,00", agenciaCentro);
		
		//Fixture Teardown
	}

	@Test
	void testarDeposito() {
		//Fixture Setup
		AuxiliarDeTestes auxiliar = new AuxiliarDeTestes(sistemaBancario);
		Conta contaMaria = auxiliar.obterContaMaria();
		Dinheiro quantia10 = new Dinheiro(Moeda.BRL, 10, 0);

		//Exercise SUT
		Operacao operacao = sistemaBancario.depositar(contaMaria, quantia10);

		//Result Verification
		assertOperacao(contaMaria, "+10,00 BRL", operacao, EstadosDeOperacao.SUCESSO);
		
		//Fixture Teardown
	}

	@Test
	void testarSaque() {
		//Fixture Setup
		AuxiliarDeTestes auxiliar = new AuxiliarDeTestes(sistemaBancario);
		Conta contaMaria = auxiliar.obterContaMaria();
		Dinheiro quantia10 = new Dinheiro(Moeda.BRL, 10, 0);
		sistemaBancario.depositar(contaMaria, quantia10);
		
		Dinheiro quantia6 = new Dinheiro(Moeda.BRL, 6, 0);
		
		//Exercise SUT
		Operacao operacao = sistemaBancario.sacar(contaMaria, quantia6);

		//Result Verification
		assertOperacao(contaMaria, "+4,00 BRL", operacao, EstadosDeOperacao.SUCESSO);
		
		//Fixture Teardown
	}

	@Test
	void testarSaqueInvalido() {
		//Fixture Setup
		AuxiliarDeTestes auxiliar = new AuxiliarDeTestes(sistemaBancario);
		Conta contaMaria = auxiliar.obterContaMaria();
		Dinheiro quantia4 = new Dinheiro(Moeda.BRL, 4, 0);
		sistemaBancario.depositar(contaMaria, quantia4);
		
		Dinheiro quantia6 = new Dinheiro(Moeda.BRL, 6, 0);
		
		//Exercise SUT
		Operacao operacao = sistemaBancario.sacar(contaMaria, quantia6);

		//Result Verification
		assertOperacao(contaMaria, "+4,00 BRL", operacao, EstadosDeOperacao.SALDO_INSUFICIENTE);
		
		//Fixture Teardown
	}

}
