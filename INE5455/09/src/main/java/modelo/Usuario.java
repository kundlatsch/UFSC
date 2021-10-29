package modelo;

import interfaces.ILeiloavel;
import interfaces.IUsuario;
import interfaces.IVendido;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Usuario implements IUsuario, Serializable {
	
	
	private static final long serialVersionUID = 1L;
	private String cpf;
	private String nome;
	private String endereco;
	private String email;
	private List<ILeiloavel> bensOfertados;
	private List<IVendido> bensComprados;
	
	
	public Usuario(String cpf, String nome) {
		this.cpf = cpf;
		this.nome = nome;
		this.bensOfertados = new ArrayList<ILeiloavel>();
		this.bensComprados = new ArrayList<IVendido>();
	}
	
	public String getNome() {
		return this.nome;
	}
	
	public String getEndereco() {
		return this.endereco;
	}
	
	public String getEmail() {
		return this.email;
	}
	
	public String getCpf() {
		return this.cpf;
	}
	
	public void setEndereco(String endereco) {
		this.endereco = endereco;
	}
	
	public void setEmail(String email) {
		this.email = email;
	}

	public List<? extends IVendido> getBensComprados() {
		return this.bensComprados;
	}
	
	public void setBemComprado(ProdutoLeilao produto) {
		this.bensComprados.add(produto);
	}

	public List<? extends ILeiloavel> getBensOfertados() {
		return this.bensOfertados;
	}
	
	public void setBemOfertado(ProdutoLeilao produto) {
		this.bensOfertados.add(produto);
	}
	
	public String toString() {
		return this.nome;
	}
}