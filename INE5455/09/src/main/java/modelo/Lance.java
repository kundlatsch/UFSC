package modelo;

import java.io.Serializable;


public class Lance implements Serializable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Double valorDoLance;
	private Usuario donoDoLance;
	private ProdutoLeilao produtoQueRecebeuOLance;
	
	public Lance(Double valorDoLance, Usuario donoDoLance) {
		this.valorDoLance = valorDoLance;
		this.donoDoLance = donoDoLance;
	}
	
	
	public Double getValorDoLance() {
		return this.valorDoLance;
	}
	
	public String getCpfDonoDoLance() {
		return this.donoDoLance.getCpf();
	}
	
	public String getNomeDonoDoLance() {
		return this.donoDoLance.getNome();
	}
	
	public ProdutoLeilao getProdutoQueRecebeuOLance() {
		return this.produtoQueRecebeuOLance;
	}
	
	public String getNomeProdutoQueRecebeuOLance() {
		return this.produtoQueRecebeuOLance.getNome();
	}
	
	public void setProdutoQueRecebeuOLance(ProdutoLeilao produto) {
		this.produtoQueRecebeuOLance = produto;
	}
	
	public String toString() {
		return "Lance no produto:  " + this.produtoQueRecebeuOLance.getNome();
	}
}
