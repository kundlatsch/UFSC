package domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import domain.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class TestesFuncionario {

    private Empresa empresaDeSoftware;
    private Projeto projetoCalculadora;
    private Funcionario gustavoKundlatsch;
    private Integer um;
    private Integer zero;

    @BeforeEach
	void setup() {
		empresaDeSoftware = new Empresa("Empresa de Software");
        projetoCalculadora = empresaDeSoftware.criarProjeto("Calculadora");
        gustavoKundlatsch = empresaDeSoftware.adicionarFuncionario("Gustavo Kundlatsch");
        um = 1;
        zero = 0;
	}

    // História de usuário:
    // Cada ocorrência tem um funcionário responsável (implementar obterNome())

    @Test
    void obterNomeFuncionario() {
        String nomeEsperado = "Gustavo Kundlatsch";
        String nomeObtido = gustavoKundlatsch.obterNome();
        assertEquals(nomeEsperado, nomeObtido);
    }

    // História de usuário:
    // Cada funcionário pode trabalhar em 
    // vários projetos simultaneamente, mas pode ser responsável por, 
    // no máximo, 10 ocorrências abertas considerando todos os projetos.

    @Test
    void obterZeroProjetosDoFuncionario() {
        Integer numeroDeProjetos = gustavoKundlatsch.obterNumeroDeProjetos();
        assertEquals(zero, numeroDeProjetos);
    }

    @Test
    void associarFuncionarioAoProjeto() {
        empresaDeSoftware.associarFuncionarioProjeto(gustavoKundlatsch, projetoCalculadora);
        Integer numeroDeProjetos = gustavoKundlatsch.obterNumeroDeProjetos();
        assertEquals(um, numeroDeProjetos);
    }

    // História de Usuário:
    // Quando uma ocorrência é criada, ela é atribuída ao seu 
    // responsável e permanece no estado aberta enquanto o seu 
    // responsável não a termina. Quando o responsável termina a 
    // ocorrência, ela é completada.

    @Test
    void associarFuncionarioAOcorrencia() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch,
            TipoOcorrencia.BUG
        );
        Integer numeroDeOcorrencias = gustavoKundlatsch.obterNumeroDeOcorrencias();
        assertEquals(um, numeroDeOcorrencias);
    }

    @Test
    void funcionarioObterOcorrenciaPorId() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        Integer ocorrenciaId = 11;
        Ocorrencia ocorrenciaObtida = gustavoKundlatsch.obterOcorrencia(ocorrenciaId);
        assertEquals(ocorrenciaObtida, falhaSoma);
    }

    @Test 
    void funcionarioFinalizarOcorrencia() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        Integer ocorrenciaId = 11;

        gustavoKundlatsch.finalizarOcorrencia(ocorrenciaId);

        Ocorrencia ocorrenciaObtida = gustavoKundlatsch.obterOcorrencia(ocorrenciaId);
        assertEquals(EstadoOcorrencia.COMPLETADA, ocorrenciaObtida.obterEstado());
    }

}