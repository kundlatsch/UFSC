package modelo;

import static org.junit.jupiter.api.Assertions.*;
import interfaces.ILeiloavel;
import interfaces.IMercadoLeilao;
import interfaces.IVendido;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class TesteOMercadoDeLeilao {

	private IMercadoLeilao mercado;
	private String nomeUsuario;
	private String enderecoUsuario;
	private String emailUsuario;
	private String cpfUsuario;
	private Date dataLimiteProduto;
	private String cpfLeiloadorProduto;
	private String nomeProduto;
	private String descricaoProduto;
	private Double valorMinimoProduto;
	private Double valorDoLance;

	@BeforeEach
	public void iniciar() {
		nomeUsuario = "Douglas";
		enderecoUsuario = "Zona Rural";
		emailUsuario = "douglas@gmail.com";
		cpfUsuario = "062.193.859-93";
		dataLimiteProduto = new Date(new Date().getTime() + 3000);
		cpfLeiloadorProduto = "111.111.111-11";
		nomeProduto = "Bola de futebol";
		descricaoProduto = "Oficial Fifa";
		valorMinimoProduto = 30.0;
		valorDoLance = 31.0;
		mercado = new MercadoLeilao();
	}

	@Test
	public void comparandoData() throws Exception {
		SimpleDateFormat formatador = new SimpleDateFormat("yyyy/dd/MM HH:mm");
		Date when = formatador.parse("2014/17/09 12:32");
		Date sistema = new Date();
		assertTrue(sistema.after(when));
	}

	@Test
	public void cadastrarUsuario() throws Exception {
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario, cpfUsuario);
	}

	@Test
	public void cadastrarProduto() throws Exception {
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario, cpfLeiloadorProduto);
		mercado.cadastrarProduto(nomeProduto, descricaoProduto, valorMinimoProduto, cpfLeiloadorProduto,
				dataLimiteProduto);
	}

	@Test
	public void produtosEmLeilao() throws Exception {
		List<? extends ILeiloavel> lista = mercado.getProdutosEmLeilao();
		assertTrue(lista.isEmpty());
	}

	@Test
	public void produtosVendidos() throws Exception {
		List<? extends IVendido> lista = mercado.getProdutosVendidos();
		assertTrue(lista.isEmpty());
	}

	@Test
	public void getProdutosQueDeuLance() throws Exception {
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario, cpfUsuario);
		List<? extends ILeiloavel> lista = mercado.getProdutosQueDeuLance(cpfUsuario);
		assertTrue(lista.isEmpty());
	}

	@Test
	public void daLance() throws Exception {
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario, cpfUsuario);
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario, cpfLeiloadorProduto);
		mercado.cadastrarProduto(nomeProduto, descricaoProduto, valorMinimoProduto, cpfLeiloadorProduto,
				dataLimiteProduto);
		mercado.daLance(nomeProduto, cpfUsuario, valorDoLance);
	}

}
