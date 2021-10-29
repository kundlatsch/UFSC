package modelo;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Date;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import interfaces.IMercadoLeilao;
import modelo.MercadoLeilao;

public class TesteMercadoComProduto {
	
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
	public void setup(){
		mercado = new MercadoLeilao();
	}
	
	@Test
	public void TesteMercadoComUmProdutoDoJoao() throws Exception {
		nomeUsuario = "Joao";
		enderecoUsuario = "Centro";
		emailUsuario = "joao@com";
		cpfLeiloadorProduto = "111.111.111-11";
		nomeProduto = "guitarra";
		descricaoProduto = "fender";
		valorMinimoProduto = 1000.0;
		dataLimiteProduto = new Date(new Date().getTime() + 3000);
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario, cpfLeiloadorProduto);
		mercado.cadastrarProduto(nomeProduto, descricaoProduto, valorMinimoProduto, cpfLeiloadorProduto, dataLimiteProduto);
		assertEquals("Joao", mercado.getUsuarioPor(cpfLeiloadorProduto).getNome());
		assertEquals("guitarra", mercado.getProdutosEmLeilao().get(0).getNome());
	}

}
