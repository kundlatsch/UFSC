package domain;

import java.util.LinkedList;

public class Empresa {

	private LinkedList<Funcionario> funcionarios;
	private LinkedList<Projeto> projetos;
	private String nome;

	protected Empresa(String nome) {
		this.nome = nome;
		this.funcionarios = new LinkedList<>();
		this.projetos = new LinkedList<>();
	}

    public Funcionario adicionarFuncionario(String nome) {
        Funcionario novoFuncionario = new Funcionario(nome);
        funcionarios.add(novoFuncionario);
        return novoFuncionario;
    }

    public Projeto criarProjeto(String nome) {
        Projeto novoProjeto = new Projeto(nome, projetos.size() + 1);
        this.projetos.add(novoProjeto);
        return novoProjeto;
    }
	
	public String obterNome() {
		return nome;
	}

    public Integer obterNumeroDeFuncionarios() {
        return funcionarios.size();
    }

    public Integer obterNumeroDeProjetos() {
        return projetos.size();
    }

	public void associarFuncionarioProjeto(Funcionario funcionario, Projeto projeto) {
        funcionario.associarProjeto(projeto);
        projeto.associarFuncionario(funcionario);
    }

}
