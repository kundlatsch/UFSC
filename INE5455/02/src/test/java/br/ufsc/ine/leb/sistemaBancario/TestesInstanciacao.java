package br.ufsc.ine.leb.sistemaBancario;

import org.junit.jupiter.api.Test;
import br.ufsc.ine.leb.sistemaBancario.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class TestesInstanciacao {

	// Nessa classe é utilizado apenas in-line setup
	// pois ela é reponsável por testar se todas as
	// funções de instânciação das classes estão funcionando
	// para que possam ser usadas nos demais testes.
	
	// Os testes foram ordenados em cascata, isto é, os testes
	// declarados primeiros instanciam as classes usadas pelos
	// métodos subsequentes.

	// Os testes poderiam agrupar instâncias parecidas em um
	// método before, mas como o SUT exercitado consiste justamente
	// na instanciação dessas classes, foi feito de maneira in-line
	// para tornar os testes mais claros.

	// Uma alternativa seria criar uma classe Helper que 
	// seria instanciada com atributos Moeda, Dinheiro, Banco e Agencia
	// como nulos, e durante os testes instâncias criadas na etapa de
	// exercitar o SUT seriam atribuídas a esses atributos.
	// Isso resultaria em um ganho de performance, pois o mesmo objeto
	// não precisaria ser instanciado várias vezes, mas os testes
	// ficariam mais confusos pois isso precisaria ser feito no
	// Fixture Teardown. Como aqui são poucos testes, preferi manter
	// dessa maneira.

	@Test
	void testarReal() {
		//Fixture Setup
		
		//Exercise SUT
		Moeda real = Moeda.BRL;
		
		//Result Verification
		assertEquals("R$", real.obterSimbolo());
		assertEquals(new Integer(100), real.obterBaseFracionaria());
		
		//Fixture Teardown
	}

	@Test
	void testarDolar() {
		//Fixture Setup
		
		//Exercise SUT
		Moeda dolar = Moeda.USD;
		
		//Result Verification
		assertEquals("$", dolar.obterSimbolo());
		assertEquals(new Integer(100), dolar.obterBaseFracionaria());
		
		//Fixture Teardown
	}

	@Test
	void testarFranco() {
		//Fixture Setup
		
		//Exercise SUT
		Moeda franco = Moeda.CHF;
		
		//Result Verification
		assertEquals("Fr", franco.obterSimbolo());
		assertEquals(new Integer(100), franco.obterBaseFracionaria());
		
		//Fixture Teardown
	}

	@Test
	void testarDinheiro() {
		//Fixture Setup
		
		//Exercise SUT
		Dinheiro dinheiro = new Dinheiro(Moeda.BRL, 100, 10);
		
		//Result Verification
		assertEquals(Moeda.BRL, dinheiro.obterMoeda());
		assertEquals("100,10 BRL", dinheiro.formatado());
		
		//Fixture Teardown
	}


	@Test
	void testarBanco() {
		//Fixture Setup
		Dinheiro taxa = new Dinheiro(Moeda.BRL, 10, 0);
		//Exercise SUT
		Banco banco = new Banco("Teste", Moeda.BRL, taxa);
		
		//Result Verification
		assertEquals("Teste", banco.obterNome());
		assertEquals(Moeda.BRL, banco.obterMoeda());

		//Fixture Teardown
	}

	@Test
	void testarAgencia() {
		//Fixture Setup
		Dinheiro taxa = new Dinheiro(Moeda.BRL, 10, 0);
		Banco banco = new Banco("Teste", Moeda.BRL, taxa);
		//Exercise SUT
		Agencia agencia = new Agencia("Agencia 01", 1, banco);
		
		//Result Verification
		assertEquals("Agencia 01", agencia.obterNome());
		assertEquals("001", agencia.obterIdentificador());
		assertEquals(banco, agencia.obterBanco());

		//Fixture Teardown
	}
	
	@Test
	void testarConta() {
		//Fixture Setup
		Dinheiro taxa = new Dinheiro(Moeda.BRL, 10, 0);
		Banco banco = new Banco("Teste", Moeda.BRL, taxa);
		Agencia agencia = new Agencia("Agencia 01", 1, banco);
		//Exercise SUT
		Conta conta = new Conta("Fulano", 1, agencia);
		
		//Result Verification
		assertEquals("Fulano", conta.obterTitular());
		assertEquals(agencia, conta.obterAgencia());
		assertEquals("0001-6", conta.obterIdentificador());

		//Fixture Teardown
	}

	@Test 
	void testarSistemaBancario() {
		//Fixture Setup

		//Exercise SUT
		SistemaBancario sistema = new SistemaBancario();

		assertEquals(new LinkedList<>(), sistema.obterBancos());

	}



}
