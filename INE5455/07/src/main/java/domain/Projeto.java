package domain;

import domain.Funcionario;
import java.util.LinkedList;

public class Projeto {

	private String nome;
	private Integer id;
	private LinkedList<Ocorrencia> ocorrencias;
	private LinkedList<Funcionario> funcionarios;

	protected Projeto(String nome, Integer id) {
		this.nome = nome;
		this.id = id;
		this.ocorrencias = new LinkedList<>();
		this.funcionarios = new LinkedList<>();
	}

	public Ocorrencia criarOcorrencia(String nome, Funcionario responsavel, TipoOcorrencia tipoOcorrencia) {
		if (responsavel.obterNumeroDeOcorrencias() >= 10) 
			return null;

		int numeroNovaOcorrencia = this.ocorrencias.size() + 1;
		String ocorrenciaId = "" + this.id + numeroNovaOcorrencia; 
		Integer ocorrenciaIdInteger = Integer.parseInt(ocorrenciaId);

		Ocorrencia novaOcorrencia = new Ocorrencia(nome, ocorrenciaIdInteger, responsavel, tipoOcorrencia);
		this.ocorrencias.add(novaOcorrencia);
		responsavel.associarOcorrencia(novaOcorrencia);
		return novaOcorrencia;
	}

    public Integer obterNumeroDeOcorrencias() {
        return ocorrencias.size();
    }

	public String obterNome() {
		return nome;
	}

	public void associarFuncionario(Funcionario funcionario) {
		if (!this.funcionarios.contains(funcionario))
			this.funcionarios.add(funcionario);
	}

}
