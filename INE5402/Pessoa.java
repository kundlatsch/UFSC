public class Pessoa{
  private String nome;
  private int cpf;
  private int idade;
  private String cidade;
  private Evento[] participacoes;

  public Pessoa(){}
  
  public Pessoa(String nome, int cpf, int idade, String nacionalidade,
		Evento[] participacoes) {
	super();
	this.nome = nome;
	this.cpf = cpf;
	this.idade = idade;
	this.cidade = nacionalidade;
	this.participacoes = participacoes;
}



public Evento[] getParticipacoes(){
    return this.participacoes;
  }

	public String getNome() {
		return this.nome;
	}

	public int getCpf() {
		return this.cpf;
	}

	public int getIdade() {
		return this.idade;
	}

	public String getNacionalidade() {
		return this.cidade;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}

	public void setCpf(int cpf) {
		this.cpf = cpf;
	}

	public void setIdade(int idade) {
		this.idade = idade;
	}

	public void setNacionalidade(String nacionalidade) {
		this.cidade = nacionalidade;
	}
}
