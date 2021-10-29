package interfaces;

import java.util.Date;
import java.util.List;

public interface IMercadoLeilao {
	public void cadastrarUsuario(String nome, String endereco, String email, String cpf) throws Exception;

	public void cadastrarProduto(String nome, String descricao, Double valorMinimo, String cpfLeiloador, Date dataLimite) throws Exception;

	public List<? extends ILeiloavel> getProdutosEmLeilao();

	public List<? extends IVendido> getProdutosVendidos();

	public void daLance(String nomeProduto, String cpfComprador, Double valorLance) throws Exception;

	/* Exceção se o CPF não for encontrado */
	public List<? extends ILeiloavel> getProdutosQueDeuLance(String cpf) throws Exception;

	public IUsuario getUsuarioPor(String cpf) throws Exception;

	public List<? extends ILeiloavel> getProdutosVencidosENaoVendidos();

	public boolean isPossuiUsuario();

	public boolean isPossuiProduto();
	
	public boolean verificaSeOProdutoJaExiste(String nome);

}