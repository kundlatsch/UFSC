package domain;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import domain.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.LinkedList;

public class TestesOcorrencia {

    private Empresa empresaDeSoftware;
    private Projeto projetoCalculadora;
    private Funcionario gustavoKundlatsch;
    private Integer um;

    @BeforeEach
	void setup() {
		empresaDeSoftware = new Empresa("Empresa de Software");
        gustavoKundlatsch = empresaDeSoftware.adicionarFuncionario("Gustavo Kundlatsch");
        projetoCalculadora = empresaDeSoftware.criarProjeto("Calculadora");
        um = 1;
	}

    // História de usuário:
    // Uma chave identifica unicamente uma ocorrência e um resumo 
    // mostra sobre o que trata a ocorrência.

    @Test
    void testarIdentificadorOcorrencia() {
        Integer ocorrenciaIdEsperado = 11;
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        Integer idObtido = falhaSoma.obterIdentificador();
        assertEquals(ocorrenciaIdEsperado, idObtido);
    }

    @Test
    void testarDescricaoOcorrencia() {
        String ocorrenciaDescricaoEsperada = "Falha na soma";
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        String descricaoObtida = falhaSoma.obterDescricao();
        assertEquals(ocorrenciaDescricaoEsperada, descricaoObtida);
    }

    // História de usuário:
    // Cada ocorrência tem um funcionário responsável

    @Test
    void testarFuncionarioOcorrencia() {
        String nomeResponsavelEsperado = "Gustavo Kundlatsch";
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        String nomeResponsavelObtido = falhaSoma.obterNomeResponsavel();
        assertEquals(nomeResponsavelEsperado, nomeResponsavelObtido);
    }

    // História de usuário:
    // Cada funcionário pode trabalhar em 
    // vários projetos simultaneamente, mas pode ser responsável por, 
    // no máximo, 10 ocorrências abertas considerando todos os projetos.

    @Test
    void testarLimiteDeOcorrenciasPorFuncionario() {
        Ocorrencia ocorrencia0 = projetoCalculadora.criarOcorrencia("Oc0", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia1 = projetoCalculadora.criarOcorrencia("Oc1", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia2 = projetoCalculadora.criarOcorrencia("Oc2", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia3 = projetoCalculadora.criarOcorrencia("Oc3", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia4 = projetoCalculadora.criarOcorrencia("Oc4", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia5 = projetoCalculadora.criarOcorrencia("Oc5", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia6 = projetoCalculadora.criarOcorrencia("Oc6", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia7 = projetoCalculadora.criarOcorrencia("Oc7", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia8 = projetoCalculadora.criarOcorrencia("Oc8", gustavoKundlatsch, TipoOcorrencia.TAREFA);
        Ocorrencia ocorrencia9 = projetoCalculadora.criarOcorrencia("Oc9", gustavoKundlatsch, TipoOcorrencia.TAREFA);

        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        
        Integer numeroOcorrencias = gustavoKundlatsch.obterNumeroDeOcorrencias();
        Integer dez = 10;
        assertEquals(null, falhaSoma);
        assertEquals(dez, numeroOcorrencias);
        assertNotEquals(null, ocorrencia9);
    }

    // História de usuário:
    // Existem diferentes tipos de ocorrências: tarefa, bug, melhoria.

    @Test
    void testarTipoOcorrencia() {
        Ocorrencia adicionarMultiplicacao = projetoCalculadora.criarOcorrencia(
            "Adicionar multiplicação", 
            gustavoKundlatsch, 
            TipoOcorrencia.TAREFA
        );
        TipoOcorrencia tipoOcorrenciaObtido = adicionarMultiplicacao.obterTipoDeOcorrencia();
        assertEquals(TipoOcorrencia.TAREFA, tipoOcorrenciaObtido);
    }

    // História de Usuário:
    // Cada ocorrência tem diferentes prioridades (alta, média, baixa) 
    // e estas prioridades podem ser modificadas.

    @Test
    void testarPrioridadeNaoAtribuida() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch,
            TipoOcorrencia.BUG
        );
        Prioridade proridadeObtida = falhaSoma.obterPrioridade();
        assertEquals(Prioridade.NAO_ATRIBUIDA, proridadeObtida);
    }

    @Test
    void atribuirPrioridade() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch, 
            TipoOcorrencia.BUG
        );
        falhaSoma.definirPrioridade(Prioridade.ALTA);
        Prioridade proridadeObtida = falhaSoma.obterPrioridade();
        assertEquals(Prioridade.ALTA, proridadeObtida);
    }

    // História de Usuário:
    // Quando uma ocorrência é criada, ela é atribuída ao seu 
    // responsável e permanece no estado aberta enquanto o seu 
    // responsável não a termina.

    @Test
    void obterEstadoInicialDaOcorrencia() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch,
            TipoOcorrencia.BUG
        );
        EstadoOcorrencia estadoObtido = falhaSoma.obterEstado();
        assertEquals(EstadoOcorrencia.ABERTA, estadoObtido);
    }

    // História de Usuário:
    // Enquanto uma ocorrência está no estado aberta, o seu 
    // responsável e a sua prioridade podem mudar. 
    
    @Test
    void alterarEstadoOcorrenciaFechada() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch,
            TipoOcorrencia.BUG
        );
        falhaSoma.definirPrioridade(Prioridade.ALTA);
        falhaSoma.definirEstado(EstadoOcorrencia.COMPLETADA);

        // A definição deve falhar e a prioridade continuar alta
        falhaSoma.definirPrioridade(Prioridade.MEDIA);

        Prioridade proridadeObtida = falhaSoma.obterPrioridade();
        assertEquals(Prioridade.ALTA, proridadeObtida);
    }

    @Test
    void alterarResponsavel() {
        Ocorrencia falhaSoma = projetoCalculadora.criarOcorrencia(
            "Falha na soma", 
            gustavoKundlatsch,
            TipoOcorrencia.BUG
        );
        Funcionario jorge = empresaDeSoftware.adicionarFuncionario("Jorge");

        falhaSoma.definirResponsavel(jorge);

        Integer ocorrenciaId = 11;
        Ocorrencia ocorrenciaObtida = jorge.obterOcorrencia(ocorrenciaId);
        assertEquals(falhaSoma, ocorrenciaObtida);
    }
    
}