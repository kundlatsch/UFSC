package domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import domain.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class TestesProjeto {

    private Empresa empresaDeSoftware;
    private Projeto projetoCalculadora;
    private Integer zero;

    @BeforeEach
	void setup() {
		empresaDeSoftware = new Empresa("Empresa de Software");
        projetoCalculadora = empresaDeSoftware.criarProjeto("Calculadora");
        zero = 0;
	}

    // História de usuário:
    // Um projeto tem uma coleção de ocorrências

    @Test
    void criarProjeto() {
        String nomeProjeto = projetoCalculadora.obterNome();
        assertEquals("Calculadora", nomeProjeto);
    }

    @Test
    void projetoSemOcorrencias() {
        Integer numeroDeOcorrencias = projetoCalculadora.obterNumeroDeOcorrencias();
        assertEquals(zero, numeroDeOcorrencias);
    }

}