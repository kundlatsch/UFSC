package modelo;

import interfaces.ILeiloavel;
import interfaces.IMercadoLeilao;
import interfaces.IUsuario;
import interfaces.IVendido;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Date;
import java.util.Map;

public class MercadoLeilao implements IMercadoLeilao, Serializable {

	private static final long serialVersionUID = 1L;
	private Map<String, Usuario> usuarios;
	private List<ProdutoLeilao> produtosEmLeilao;
	private List<ProdutoLeilao> produtosVendidos;
	private List<ProdutoLeilao> produtosVencidosENaoVendidos;

	public MercadoLeilao() {
		this.usuarios = new HashMap<String, Usuario>();
		this.produtosEmLeilao = new ArrayList<ProdutoLeilao>();
		this.produtosVendidos = new ArrayList<ProdutoLeilao>();
		this.produtosVencidosENaoVendidos = new ArrayList<ProdutoLeilao>();
	}

	public void cadastrarUsuario(String nome, String endereco, String email, String cpf) throws Exception {
		if (verificaSeOUsuarioJaExiste(cpf)) {
			throw new Exception("O usuario já existe.");
		} else {
			if (!isCPF(cpf)){
				throw new Exception("CPF inválido: " + cpf);
			} else {
				Usuario usuario = new Usuario(cpf, nome);
				usuario.setEndereco(endereco);
				usuario.setEmail(email);
				this.usuarios.put(cpf, usuario);
			}		
		}
	}

	public void cadastrarProduto(String nome, String descricao, Double lanceMinimo, String cpfLeiloador,
			Date dataLimite) throws Exception {
		if (!verificaSeOProdutoJaExiste(nome) && verificaSeOUsuarioJaExiste(cpfLeiloador)) {
			Usuario leiloador = usuarios.get(cpfLeiloador);
			ProdutoLeilao produto = new ProdutoLeilao(nome, descricao, lanceMinimo, leiloador);
			produto.setDataLimite(dataLimite);
			produtosEmLeilao.add(produto);
			leiloador.setBemOfertado(produto);
		} else
			throw new Exception("O produto ja existe ou o leiloador nao esta cadastrado.");
	}

	public List<? extends ILeiloavel> getProdutosEmLeilao() {
		atualizarListasDeProdutos();
		List<ILeiloavel> retornoProdutosEmLeilao = new ArrayList<ILeiloavel>();
		retornoProdutosEmLeilao.addAll(this.produtosEmLeilao);
		return retornoProdutosEmLeilao;
	}

	public List<? extends ILeiloavel> getProdutosVencidosENaoVendidos() {
		atualizarListasDeProdutos();
		List<ILeiloavel> retornoProdutosVencidos = new ArrayList<ILeiloavel>();
		retornoProdutosVencidos.addAll(this.produtosVencidosENaoVendidos);
		return retornoProdutosVencidos;
	}

	public List<? extends IVendido> getProdutosVendidos() {
		atualizarListasDeProdutos();
		List<IVendido> retornoProdutosVendidos = new ArrayList<IVendido>();
		retornoProdutosVendidos.addAll(this.produtosVendidos);
		return retornoProdutosVendidos;
	}

	public List<IUsuario> getUsuariosCadastrados() {
		List<IUsuario> retornoUsuariosCadastrados = new ArrayList<IUsuario>();
		retornoUsuariosCadastrados.addAll(this.usuarios.values());
		return retornoUsuariosCadastrados;
	}

	public void daLance(String nomeProduto, String cpfComprador, Double valorLance) throws Exception {
		atualizarListasDeProdutos();
		Lance lance = new Lance(valorLance, (Usuario) this.getUsuarioPor(cpfComprador));
		ProdutoLeilao produto = produtosEmLeilao.get(pesquisaIndexProdutoEmLeilaoViaNome(nomeProduto));
		Double lanceMinimo = produto.getLanceMinimo();
		Double lanceAtual = produto.getValorUltimoLance();
		if (verificaSeOUsuarioJaExiste(cpfComprador) && valorLance >= lanceMinimo && valorLance > lanceAtual) {
			produto.recebaLance(lance);
			lance.setProdutoQueRecebeuOLance(produto);
		} else
			throw new Exception("O valor do lance eh inferior ao necessario ou o comprador nao esta cadastrado.");
	}

	public List<Lance> retornaTodosOsLancesEfetuados() {
		List<Lance> retornoLances = new ArrayList<Lance>();
		retornoLances.addAll(retornaTodosOsLancesEfetuadosEmProdutosEmLeilao());
		retornoLances.addAll(retornaTodosOsLancesEfetuadosEmProdutosVendidos());
		return retornoLances;
	}

	public List<Lance> retornaLancesDeUmUsuario(String cpfUsuario) throws Exception {
		if (!verificaSeOUsuarioJaExiste(cpfUsuario))
			throw new Exception("O usuario nao esta cadastrado.");
		List<Lance> retornoLances = new ArrayList<Lance>();
		retornoLances.addAll(retornaLancesDeUmUsuarioEmProdutosAindaEmLeilao(cpfUsuario));
		retornoLances.addAll(retornaLancesDeUmUsuarioEmProdutosVendidos(cpfUsuario));
		return retornoLances;
	}

	public List<ProdutoLeilao> retornaProdutosDeUmLeiloador(String cpfUsuario) throws Exception {
		atualizarListasDeProdutos();
		if (!verificaSeOUsuarioJaExiste(cpfUsuario))
			throw new Exception("O usuario nao esta cadastrado.");
		List<ProdutoLeilao> retornoProdutos = new ArrayList<ProdutoLeilao>();
		retornoProdutos.addAll(retornaProdutosEmLeilaoPorUmUsuario(cpfUsuario));
		retornoProdutos.addAll(retornaProdutosVendidosPorUmUsuario(cpfUsuario));
		retornoProdutos.addAll(retornaProdutosVencidosMasNaoVendidosPorUmUsuario(cpfUsuario));
		return retornoProdutos;
	}

	public List<? extends ILeiloavel> getProdutosQueDeuLance(String cpf) throws Exception {
		atualizarListasDeProdutos();
		if (!verificaSeOUsuarioJaExiste(cpf))
			throw new Exception("O usuario nao esta cadastrado.");
		else {
			List<ILeiloavel> produtosQueDeuLance = new ArrayList<ILeiloavel>();
			produtosQueDeuLance.addAll(getProdutosEmLeilaoQueDeuLance(cpf));
			produtosQueDeuLance.addAll(getProdutosVendidosQueDeuLance(cpf));
			return produtosQueDeuLance;
		}
	}

	public IUsuario getUsuarioPor(String cpf) throws Exception {
		IUsuario usuario = this.usuarios.get(cpf);
		if (usuario == null)
			throw new Exception("O usuario nao esta cadastrado.");
		return usuario;
	}

	/////////////////////////////////// METODOS PRIVADOS
	/////////////////////////////////// ///////////////////////////////////

	private void atualizarListasDeProdutos() {
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			ProdutoLeilao produto = produtosEmLeilao.get(i);
			atualizaSeFoiVendido(produto);
			atualizaSeVenceuENaoFoiVendido(produto);
		}
	}

	private void atualizaSeFoiVendido(ProdutoLeilao produto) {
		int qtdLances = produto.retornaTodosOsLancesFeitosNesseProduto().size();
		if (produto.dataDoProdutoExpirou() && qtdLances > 0) {
			produtosVendidos.add(produto);
			String cpfDonoDoLance = produto.getLanceMaisRecente().getCpfDonoDoLance();
			Usuario comprador = usuarios.get(cpfDonoDoLance);
			comprador.setBemComprado(produto);
			produto.setComprador(comprador);
			produtosEmLeilao.remove(produto);
		}
	}

	private void atualizaSeVenceuENaoFoiVendido(ProdutoLeilao produto) {
		int qtdLances = produto.retornaTodosOsLancesFeitosNesseProduto().size();
		if (produto.dataDoProdutoExpirou() && qtdLances == 0) {
			produtosVencidosENaoVendidos.add(produto);
			produtosEmLeilao.remove(produto);
		}
	}

	public boolean verificaSeOProdutoJaExiste(String nome) {
		boolean entreOsProdutosEmLeilao = verificaSeExisteEntreOsEmLeilao(nome);
		boolean entreOsProdutosVendidos = verificaSeExisteEntreOsVendidos(nome);
		boolean entreOsProdutosVencidos = verificaSeExisteEntreOsVencidosENaoVendidos(nome);
		return entreOsProdutosEmLeilao || entreOsProdutosVendidos || entreOsProdutosVencidos;
	}

	private boolean verificaSeExisteEntreOsEmLeilao(String nome) {
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			if (nome.equalsIgnoreCase(produtosEmLeilao.get(i).getNome())) {
				return true;
			}
		}
		return false;
	}

	private boolean verificaSeExisteEntreOsVendidos(String nome) {
		for (int i = 0; i < produtosVendidos.size(); i++) {
			if (nome.equalsIgnoreCase(produtosVendidos.get(i).getNome())) {
				return true;
			}
		}
		return false;
	}

	private boolean verificaSeExisteEntreOsVencidosENaoVendidos(String nome) {
		for (int i = 0; i < produtosVencidosENaoVendidos.size(); i++) {
			if (nome.equalsIgnoreCase(produtosVencidosENaoVendidos.get(i).getNome())) {
				return true;
			}
		}
		return false;
	}

	private boolean verificaSeOUsuarioJaExiste(String cpfUsuario) {
		return (usuarios.containsKey(cpfUsuario));
	}

	private Integer pesquisaIndexProdutoEmLeilaoViaNome(String nomeProduto) throws Exception {
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			if (nomeProduto.equalsIgnoreCase(produtosEmLeilao.get(i).nome()))
				return i;
		}
		throw new Exception("Nao existe produto cadastrado com esse nome.");
	}

	private List<Lance> retornaLancesDeUmUsuarioEmProdutosAindaEmLeilao(String cpfUsuario) {
		List<Lance> retornoLances = new ArrayList<Lance>();
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			retornoLances.addAll(produtosEmLeilao.get(i).verificaLancesEfetuadosPorUmUsuario(cpfUsuario));
		}
		return retornoLances;
	}

	private List<Lance> retornaLancesDeUmUsuarioEmProdutosVendidos(String cpfUsuario) {
		List<Lance> retornoLances = new ArrayList<Lance>();
		for (int i = 0; i < produtosVendidos.size(); i++) {
			retornoLances.addAll(produtosVendidos.get(i).verificaLancesEfetuadosPorUmUsuario(cpfUsuario));
		}
		return retornoLances;
	}

	private List<Lance> retornaTodosOsLancesEfetuadosEmProdutosEmLeilao() {
		List<Lance> retornoLances = new ArrayList<Lance>();
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			retornoLances.addAll(produtosEmLeilao.get(i).retornaTodosOsLancesFeitosNesseProduto());
		}
		return retornoLances;
	}

	private List<Lance> retornaTodosOsLancesEfetuadosEmProdutosVendidos() {
		List<Lance> retornoLances = new ArrayList<Lance>();
		for (int i = 0; i < produtosVendidos.size(); i++) {
			retornoLances.addAll(produtosVendidos.get(i).retornaTodosOsLancesFeitosNesseProduto());
		}
		return retornoLances;
	}

	private List<ProdutoLeilao> retornaProdutosEmLeilaoPorUmUsuario(String cpfUsuario) {
		List<ProdutoLeilao> retornoProdutos = new ArrayList<ProdutoLeilao>();
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			if (cpfUsuario.equals(produtosEmLeilao.get(i).getCpfLeiloador()))
				retornoProdutos.add(produtosEmLeilao.get(i));
		}
		return retornoProdutos;
	}

	private List<ProdutoLeilao> retornaProdutosVendidosPorUmUsuario(String cpfUsuario) {
		List<ProdutoLeilao> retornoProdutos = new ArrayList<ProdutoLeilao>();
		for (int i = 0; i < produtosVendidos.size(); i++) {
			if (cpfUsuario.equals(produtosVendidos.get(i).getCpfLeiloador()))
				retornoProdutos.add(produtosVendidos.get(i));
		}
		return retornoProdutos;
	}

	private List<ProdutoLeilao> retornaProdutosVencidosMasNaoVendidosPorUmUsuario(String cpfUsuario) {
		List<ProdutoLeilao> retornoProdutos = new ArrayList<ProdutoLeilao>();
		for (int i = 0; i < produtosVencidosENaoVendidos.size(); i++) {
			if (cpfUsuario.equals(produtosVencidosENaoVendidos.get(i).getCpfLeiloador()))
				retornoProdutos.add(produtosVencidosENaoVendidos.get(i));
		}
		return retornoProdutos;
	}

	private boolean verificaSeDeuLanceNesseProduto(String cpfComprador, ProdutoLeilao produto) {
		List<Lance> lances = produto.retornaTodosOsLancesFeitosNesseProduto();
		for (int i = 0; i < lances.size(); i++) {
			if (lances.get(i).getCpfDonoDoLance().equalsIgnoreCase(cpfComprador))
				return true;
		}
		return false;
	}

	private List<? extends ILeiloavel> getProdutosEmLeilaoQueDeuLance(String cpf) {
		List<ILeiloavel> retornoProdutos = new ArrayList<ILeiloavel>();
		for (int i = 0; i < produtosEmLeilao.size(); i++) {
			ProdutoLeilao produto = produtosEmLeilao.get(i);
			if (verificaSeDeuLanceNesseProduto(cpf, produto))
				retornoProdutos.add(produto);
		}
		return retornoProdutos;
	}

	private List<? extends ILeiloavel> getProdutosVendidosQueDeuLance(String cpf) {
		List<ILeiloavel> retornoProdutos = new ArrayList<ILeiloavel>();
		for (int i = 0; i < produtosVendidos.size(); i++) {
			ProdutoLeilao produto = produtosVendidos.get(i);
			if (verificaSeDeuLanceNesseProduto(cpf, produto))
				retornoProdutos.add(produto);
		}
		return retornoProdutos;
	}

	public boolean isCPF(String strCPF){  
	      if ( strCPF == null ){
	          return false;
	      }
	      else{
	              String cpfGerado = "";     
	              String cpf = removerCaracteres(strCPF);            
	          if ( cpf.length()!=11 )
	               return false;                 
	          cpfGerado = cpf.substring(0, 9);
	          cpfGerado = cpfGerado.concat(calculoComCpf(cpfGerado));
	          cpfGerado = cpfGerado.concat(calculoComCpf(cpfGerado));
	          
	          if ( !cpfGerado.equals(cpf))
	               return false;
	      } 
	      return true;
	  }
	
	 private String removerCaracteres(String cpf){   
		 return cpf.replaceAll("[^0-9]", "");
	 } 
	 
	 private String calculoComCpf(String cpf){   
		   int digGerado = 0;
		   int mult = cpf.length()+1;
		   char [] charCpf = cpf.toCharArray();
		   for ( int i = 0; i < cpf.length(); i++ )
		        digGerado += (charCpf[i]-48)* mult--;
		   if ( digGerado % 11 < 2)
		        digGerado = 0;
		   else
		       digGerado = 11 - digGerado % 11;
		   return  String.valueOf(digGerado); 
		  }

	@Override
	public boolean isPossuiUsuario() {
		// TODO Auto-generated method stub
		return !this.usuarios.isEmpty();
	}

	@Override
	public boolean isPossuiProduto() {
		if (this.produtosEmLeilao.isEmpty() && this.produtosVendidos.isEmpty()
				&& this.produtosVencidosENaoVendidos.isEmpty()) {
			return false;
		}
		return true;
	}
}