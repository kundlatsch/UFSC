package domain;

import java.util.LinkedList;

public class Ocorrencia {

	private Integer identificador;
	private String descricao;
	private Funcionario responsavel;
	private TipoOcorrencia tipoOcorrencia;
	private Prioridade prioridade;
	private EstadoOcorrencia estado;

	protected Ocorrencia(String descricao, Integer identificador, Funcionario responsavel, TipoOcorrencia tipoOcorrencia) {
        this.identificador = identificador;
		this.descricao = descricao;
		this.responsavel = responsavel;
		this.tipoOcorrencia = tipoOcorrencia;
		this.prioridade = Prioridade.NAO_ATRIBUIDA;
		this.estado = EstadoOcorrencia.ABERTA;
	}

	public Integer obterIdentificador() {
		return this.identificador;
	}

	public String obterDescricao() {
		return this.descricao;
	}

	public String obterNomeResponsavel() {
		return this.responsavel.obterNome();
	}

	public TipoOcorrencia obterTipoDeOcorrencia() {
		return this.tipoOcorrencia;
	}

	public Prioridade obterPrioridade() {
		return this.prioridade;
	}

	public void definirPrioridade(Prioridade prioridade) {
		if (this.estado == EstadoOcorrencia.ABERTA)
			this.prioridade = prioridade;
	}

	public EstadoOcorrencia obterEstado() {
		return this.estado;
	}

	public void definirEstado(EstadoOcorrencia estado) {
		this.estado = estado;
	}

	public void definirResponsavel(Funcionario funcionario) {
		this.responsavel.removerOcorrencia(this.identificador);
		this.responsavel = funcionario;
		this.responsavel.associarOcorrencia(this);
	}

}
