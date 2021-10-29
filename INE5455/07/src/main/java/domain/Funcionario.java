package domain;

import java.util.LinkedList;

public class Funcionario {

	private String nome;
	private LinkedList<Projeto> projetos;
	private LinkedList<Ocorrencia> ocorrencias;

	protected Funcionario(String nome) {
		this.nome = nome;
		this.projetos = new LinkedList<>();
		this.ocorrencias = new LinkedList<>();
	}

	public String obterNome() {
		return nome;
	}

	public Integer obterNumeroDeProjetos() {
        return projetos.size();
    }

	public void associarProjeto(Projeto projeto) {
		if (!this.projetos.contains(projeto))
			this.projetos.add(projeto);
	}

	public void associarOcorrencia(Ocorrencia ocorrencia) {
		if (!this.ocorrencias.contains(ocorrencia))
			this.ocorrencias.add(ocorrencia);
	}

	public Integer obterNumeroDeOcorrencias() {
		return ocorrencias.size();
	}

	public Ocorrencia obterOcorrencia (Integer id) {
		for (int i = 0; i < this.ocorrencias.size(); i++) {
			Ocorrencia ocorrencia = ocorrencias.get(i);
            if (ocorrencia.obterIdentificador() == id)
				return ocorrencia;
        }
		return null;
	}

	public void finalizarOcorrencia(Integer id) {
		Ocorrencia ocorrencia = obterOcorrencia(id);
		ocorrencia.definirEstado(EstadoOcorrencia.COMPLETADA);
	}

	public void removerOcorrencia(Integer id) {
		for (int i = 0; i < this.ocorrencias.size(); i++) {
			Ocorrencia ocorrencia = ocorrencias.get(i);
            if (ocorrencia.obterIdentificador() == id)
				this.ocorrencias.remove(i);
        }
	}

}
