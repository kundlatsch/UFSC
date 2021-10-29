package br.ufsc.ine.leb.sistemaBancario;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import br.ufsc.ine.leb.sistemaBancario.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class TestesSistemaBancario {

    // Para os testes do sistema bancário, foi utilizado o
    // método de implicit setup, pois os testes desta
    // classe necessitam de pelo menos uma conta. Assim, a cadeia
    // de criação de uma conta é realizada no método setup, que 
    // é executado antes dos testes, para que as contas possam ser
    // acessadas livremente.

    // Valores auxiliares de quantia de dinheiro, que são instância
    // da classe Dinheiro, foram criados com chamadas in-line pois
    // necessitam de valores diferentes de acordo com o teste.

    private SistemaBancario sistemaBancario;
    private Banco banco;
    private Agencia agencia;
    private Conta contaFulano;
    private Conta contaBeltrano;

    @BeforeEach
    void setup() {
        sistemaBancario = new SistemaBancario();
        banco = sistemaBancario.criarBanco("Banco Teste", Moeda.BRL);
        agencia = banco.criarAgencia("Agencia");
        contaFulano = agencia.criarConta("Fulano");
        contaBeltrano = agencia.criarConta("Beltrano");
    }

    @Test
	void depositar() {
		//Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);

		//Exercise SUT
		sistemaBancario.depositar(contaFulano, quantia100);
		
		//Result Verification
        assertEquals("+100,00 BRL", contaFulano.calcularSaldo().toString());
		
		//Fixture Teardown
	}

    @Test
	void sacar() {
		//Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);
		sistemaBancario.depositar(contaFulano, quantia100);

        Dinheiro quantia50 = new Dinheiro(Moeda.BRL, 50, 0);

		//Exercise SUT
		sistemaBancario.sacar(contaFulano, quantia50);
		
		//Result Verification
        assertEquals("+50,00 BRL", contaFulano.calcularSaldo().toString());
		
		//Fixture Teardown
	}

    @Test
    void zerarConta() {
        //Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);
		sistemaBancario.depositar(contaFulano, quantia100);

		//Exercise SUT
		sistemaBancario.sacar(contaFulano, quantia100);
		
		//Result Verification
        assertEquals("0,00", contaFulano.calcularSaldo().toString());
		
		//Fixture Teardown
    }

    @Test
    void transferir() {
        //Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);
		sistemaBancario.depositar(contaFulano, quantia100);

        Dinheiro quantia30 = new Dinheiro(Moeda.BRL, 30, 0);

		//Exercise SUT
		sistemaBancario.transferir(contaFulano, contaBeltrano, quantia30);
		
		//Result Verification
        assertEquals("+70,00 BRL", contaFulano.calcularSaldo().toString());
        assertEquals("+30,00 BRL", contaBeltrano.calcularSaldo().toString());
		
		//Fixture Teardown
    }

    @Test
    void autotransferencia() {
        //Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);
		sistemaBancario.depositar(contaFulano, quantia100);

        Dinheiro quantia30 = new Dinheiro(Moeda.BRL, 30, 0);

		//Exercise SUT
		sistemaBancario.transferir(contaFulano, contaFulano, quantia30);
		
		//Result Verification
        assertEquals("+100,00 BRL", contaFulano.calcularSaldo().toString());
		
		//Fixture Teardown
    }



}