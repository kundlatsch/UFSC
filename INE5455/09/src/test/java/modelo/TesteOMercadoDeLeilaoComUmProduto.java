package modelo;

import static org.junit.jupiter.api.Assertions.*;
import interfaces.ILeiloavel;
import interfaces.IMercadoLeilao;

import java.util.Date;
import java.util.List;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class TesteOMercadoDeLeilaoComUmProduto {

	private IMercadoLeilao mercado;
	private Date dataLimiteProduto;
	private String cpfLeiloadorProduto;
	private String nomeProduto;
	private String descricaoProduto;
	private Double valorMinimoProduto;
	private String nomeUsuario;
	private String enderecoUsuario;
	private String emailUsuario;

	@BeforeEach
	public void iniciar() throws Exception {
		nomeUsuario = "Douglas";
		enderecoUsuario = "Zona Rural";
		emailUsuario = "douglas@gmail.com";
		cpfLeiloadorProduto = "111.111.111-11";
		dataLimiteProduto = new Date(new Date().getTime() + 3000);
		nomeProduto = "Bola de futebol";
		descricaoProduto = "Oficial Fifa";
		valorMinimoProduto = 30.0;
		mercado = new MercadoLeilao();
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario,
				cpfLeiloadorProduto);
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto, dataLimiteProduto);
	}

	@Test
	public void produto() throws Exception {
		List<? extends ILeiloavel> produtos = mercado.getProdutosEmLeilao();
		assertEquals(1, produtos.size());
		assertEquals(nomeProduto, produtos.get(0).getNome());
		assertEquals(cpfLeiloadorProduto, produtos.get(0).getCpfLeiloador());
		assertEquals(descricaoProduto, produtos.get(0).getDescricao());
	}

	@Test
	//(expected = Exception.class)
	public void produtoSemLance() throws Exception {
		List<? extends ILeiloavel> produtos = mercado.getProdutosEmLeilao();
		produtos.get(0).getValorUltimoLance();
	}

	@Test
	public void produtoVencido() throws Exception {
		String nomeProdutoVencido = "Vencido";
		mercado.cadastrarProduto(nomeProdutoVencido, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 100));
		Thread.sleep(2000);
		assertEquals(1, mercado.getProdutosEmLeilao().size());
		assertEquals(1, mercado.getProdutosVencidosENaoVendidos().size());
		assertEquals(0, mercado.getProdutosVendidos().size());

		assertEquals(nomeProduto, mercado.getProdutosEmLeilao().get(0)
				.getNome());
		assertEquals(nomeProdutoVencido, mercado
				.getProdutosVencidosENaoVendidos().get(0).getNome());
	}

}