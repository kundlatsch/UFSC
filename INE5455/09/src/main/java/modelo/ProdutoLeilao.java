package modelo;

import interfaces.IVendido;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class ProdutoLeilao extends Produto implements IVendido, Serializable  {

	
	/**
	 * 
	 */
	private static final long serialVersionUID = 12L;
	private Date dataLimite;
	private Double lanceMinimo;
	private List<Lance> lancesEfetuados;
	private Usuario leiloador;
	private Usuario comprador;
 
	
	public ProdutoLeilao(String nome, String descricao, Double lanceMinimo, Usuario leiloador) {
		super(nome, descricao);
		this.lanceMinimo = lanceMinimo;
		this.leiloador = leiloador;
		this.lancesEfetuados = new ArrayList<Lance>();
	}
		
	public Date getDataLimite() {
		return this.dataLimite;
	}
	
	public void setDataLimite(Date data) {
		this.dataLimite = data;
	}
	
	public Double getLanceMinimo() {
		return this.lanceMinimo;
	}
	
	public List<Lance> retornaTodosOsLancesFeitosNesseProduto() {
		return this.lancesEfetuados;
	}
	
	public void recebaLance(Lance lance) {
			this.lancesEfetuados.add(lance);
	}
	
	public boolean dataDoProdutoExpirou() {
		return new Date().after(this.dataLimite);
	}
	
	public List<Lance> verificaLancesEfetuadosPorUmUsuario(String cpfUsuario) {
		List<Lance> retornoDeLances = new ArrayList<Lance>();
		for(int i=0; i<lancesEfetuados.size(); i++) {
			if(cpfUsuario.equals(lancesEfetuados.get(i).getCpfDonoDoLance())) {
				retornoDeLances.add(lancesEfetuados.get(i));
			}
		}
		return retornoDeLances;
	}

	public String getNome() {
		return this.nome();
	}
	
	public String getDescricao() {
		return this.descricao();
	}
	
	public String getCpfLeiloador() {
		return this.leiloador.getCpf();
	}
	
	public Double getValorUltimoLance() {
		try {
			int index = this.lancesEfetuados.size()-1;
			Lance lance = this.lancesEfetuados.get(index);
			return lance.getValorDoLance();
		}catch(RuntimeException e) {
			//e.printStackTrace();
			return 0.0;
		}
		
	}
	
	public Lance getLanceMaisRecente() {
		try {
			int index = this.lancesEfetuados.size()-1;
			return this.lancesEfetuados.get(index);
		}catch(Exception e) {
			e.printStackTrace();
			return new Lance(0.0, new Usuario("", ""));
		}
	}
	
	public String getCpfComprador() {
		return this.comprador.getCpf();
	}
	
	public void setComprador(Usuario comprador) {
		this.comprador = comprador;
	}
	
	public String toString() {
		return this.nome();
	}
}