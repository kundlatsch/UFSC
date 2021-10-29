package domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import domain.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class TestesEmpresa {

    private Empresa empresaDeSoftware;
    private Integer um;

    @BeforeEach
	void setup() {
		empresaDeSoftware = new Empresa("Empresa de Software");
        um = 1;
	}

    // História de usuário:
    // Uma empresa possui vários funcionários e desenvolve vários projetos

    @Test
	void obterNomeDaEmpresa() {
        String nomeDaEmpresa = empresaDeSoftware.obterNome();
        assertEquals("Empresa de Software", nomeDaEmpresa);
    }

    @Test
    void adicionarFuncionario() {
        empresaDeSoftware.adicionarFuncionario("Gustavo Kundlatsch");
        Integer numeroDeFuncionarios = empresaDeSoftware.obterNumeroDeFuncionarios();
        assertEquals(um, numeroDeFuncionarios);
    }

    @Test
    void criarProjeto() {
        empresaDeSoftware.criarProjeto("Calculadora");
        Integer numeroDeProjetos = empresaDeSoftware.obterNumeroDeProjetos();
        assertEquals(um, numeroDeProjetos);
    }

}