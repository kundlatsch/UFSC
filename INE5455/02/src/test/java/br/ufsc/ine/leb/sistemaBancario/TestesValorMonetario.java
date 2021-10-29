package br.ufsc.ine.leb.sistemaBancario;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import br.ufsc.ine.leb.sistemaBancario.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;


public class TestesValorMonetario {

    // Para  os testes da classe ValorMonetario, foi utilizado
    // a técnica de delegated setup para padronizar a criação
    // de instâncias da classe, uma vez que diversos testes
    // precisam ser executados com ela, em diferentes configurações.
    // A classe AuxiliarDeTestes foi criada para isso.
    
    @Test
	void somar() {
		//Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);
        ValorMonetario vm = AuxiliarDeTestes.criarVM();

		//Exercise SUT
		ValorMonetario resultado = vm.somar(quantia100);
		
		//Result Verification
        assertEquals("+100,00 BRL", resultado.toString());
		
		//Fixture Teardown
	}

    @Test
    void subtrair() {
        //Fixture Setup
        Dinheiro quantia100 = new Dinheiro(Moeda.BRL, 100, 0);
        ValorMonetario vm = AuxiliarDeTestes.criarVM();

		//Exercise SUT
		ValorMonetario resultado = vm.subtrair(quantia100);
		
		//Result Verification
        assertEquals("-100,00 BRL", resultado.toString());
		
		//Fixture Teardown
    }

    @Test
    void verificarNegativo() {
        //Fixture Setup
        ValorMonetario vmNegativo = AuxiliarDeTestes.criarVMNegativo();
		
		//Exercise SUT and Result Verification
        assertEquals("-50,00 BRL", vmNegativo.toString());
		
		//Fixture Teardown
    }

    @Test
    void verificarIgualdadeIncorreta() {
        //Fixture Setup
        ValorMonetario vmPositivo = AuxiliarDeTestes.criarVMPositivo();
        ValorMonetario vmNegativo = AuxiliarDeTestes.criarVMNegativo();

		//Exercise SUT
        Boolean igualdade = vmPositivo.equals(vmNegativo);

        //Result Verification
        assertEquals(false, igualdade);

		//Fixture Teardown
    }

    @Test
    void verificarIgualdadeCorreta() {
        //Fixture Setup
        ValorMonetario vmPositivo1 = AuxiliarDeTestes.criarVMPositivo();
        ValorMonetario vmPositivo2 = AuxiliarDeTestes.criarVMPositivo();

		//Exercise SUT
        Boolean igualdade = vmPositivo1.equals(vmPositivo2);
        
        //Result Verification
        assertEquals(true, igualdade);

		//Fixture Teardown
    }

}