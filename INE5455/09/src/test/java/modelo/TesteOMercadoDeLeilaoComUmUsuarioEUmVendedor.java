package modelo;

import static org.junit.jupiter.api.Assertions.*;
import interfaces.ILeiloavel;
import interfaces.IMercadoLeilao;
import interfaces.IUsuario;
import interfaces.IVendido;

import java.util.Date;
import java.util.List;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class TesteOMercadoDeLeilaoComUmUsuarioEUmVendedor {

	private IMercadoLeilao mercado;
	private String nomeUsuario;
	private String enderecoUsuario;
	private String emailUsuario;
	private String cpfUsuario;
	private String cpfLeiloadorProduto;
	private String nomeProduto;
	private String descricaoProduto;
	private Double valorMinimoProduto;
	private Double valorDoLance;

	@BeforeEach
	public void iniciar() throws Exception {
		nomeUsuario = "Douglas";
		enderecoUsuario = "Zona Rural";
		emailUsuario = "douglas@gmail.com";
		cpfUsuario = "062.193.859-93";
		cpfLeiloadorProduto = "111.111.111-11";
		nomeProduto = "Bola de futebol";
		descricaoProduto = "Oficial Fifa";
		valorMinimoProduto = 30.0;
		valorDoLance = 31.0;
		mercado = new MercadoLeilao();
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario,
				cpfLeiloadorProduto);
		mercado.cadastrarUsuario(nomeUsuario, enderecoUsuario, emailUsuario,
				cpfUsuario);

	}

	@Test
	public void obterUmLeiloador() throws Exception {
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 5000));
		IUsuario usuario = mercado.getUsuarioPor(cpfLeiloadorProduto);
		assertEquals(cpfLeiloadorProduto, usuario.getCpf());
		assertEquals(emailUsuario, usuario.getEmail());
		assertEquals(enderecoUsuario, usuario.getEndereco());
		assertEquals(0, usuario.getBensComprados().size());
		assertEquals(1, usuario.getBensOfertados().size());
		ILeiloavel produto = usuario.getBensOfertados().get(0);
		assertEquals(nomeProduto, produto.getNome());
		assertEquals(cpfLeiloadorProduto, produto.getCpfLeiloador());
		assertEquals(descricaoProduto, produto.getDescricao());
	}

	@Test
	public void obterUmUsuario() throws Exception {
		IUsuario usuario = mercado.getUsuarioPor(cpfUsuario);
		assertEquals(cpfUsuario, usuario.getCpf());
		assertEquals(emailUsuario, usuario.getEmail());
		assertEquals(enderecoUsuario, usuario.getEndereco());
		assertEquals(0, usuario.getBensComprados().size());
		assertEquals(0, usuario.getBensOfertados().size());
	}

	@Test
	public void ultimoLanceMenorQueOValorDoProduto() throws Exception {
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 5000));
		try {
			mercado.daLance(nomeProduto, cpfUsuario, valorMinimoProduto - 1);
			fail("Valor Minimo!");
		} catch (Exception e) {
		}
		List<? extends ILeiloavel> lancados = mercado
				.getProdutosQueDeuLance(cpfUsuario);
		assertEquals(0, lancados.size());
		assertEquals(1, mercado.getProdutosEmLeilao().size());
		assertEquals(nomeProduto, mercado.getProdutosEmLeilao().get(0)
				.getNome());
		assertEquals(0, mercado.getProdutosVencidosENaoVendidos().size());
		assertEquals(0, mercado.getProdutosVendidos().size());
	}

	@Test
	public void ultimoLance() throws Exception {
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 5000));
		mercado.daLance(nomeProduto, cpfUsuario, valorDoLance);
		List<? extends ILeiloavel> lancados = mercado
				.getProdutosQueDeuLance(cpfUsuario);
		assertEquals(1, lancados.size());
		assertEquals(nomeProduto, lancados.get(0).getNome());
		assertEquals(valorDoLance, lancados.get(0).getValorUltimoLance());
		assertEquals(cpfLeiloadorProduto, lancados.get(0).getCpfLeiloador());
		assertEquals(1, mercado.getProdutosEmLeilao().size());
		assertEquals(nomeProduto, mercado.getProdutosEmLeilao().get(0)
				.getNome());
		assertEquals(0, mercado.getProdutosVencidosENaoVendidos().size());
		assertEquals(0, mercado.getProdutosVendidos().size());
	}

	@Test
	public void produtoVencido() throws Exception {
		String nomeProdutoVencido = "Vencido";
		mercado.cadastrarProduto(nomeProdutoVencido, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 100));
		Thread.sleep(2000);
		assertEquals(0, mercado.getProdutosEmLeilao().size());
		assertEquals(1, mercado.getProdutosVencidosENaoVendidos().size());
		assertEquals(0, mercado.getProdutosVendidos().size());
		assertEquals(nomeProdutoVencido, mercado
				.getProdutosVencidosENaoVendidos().get(0).getNome());
	}

	@Test
	public void produtoComprado() throws Exception {
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 1000));
		mercado.daLance(nomeProduto, cpfUsuario, valorDoLance);
		Thread.sleep(2000);
		assertEquals(0, mercado.getProdutosEmLeilao().size());
		assertEquals(0, mercado.getProdutosVencidosENaoVendidos().size());
		List<? extends IVendido> vendidos = mercado.getProdutosVendidos();
		assertEquals(1, vendidos.size());
		assertEquals(nomeProduto, vendidos.get(0).getNome());
		assertEquals(cpfUsuario, vendidos.get(0).getCpfComprador());
		assertEquals(valorDoLance, vendidos.get(0).getValorUltimoLance());
		List<? extends ILeiloavel> lances = mercado
				.getProdutosQueDeuLance(cpfUsuario);
		assertEquals(1, lances.size());
		assertEquals(nomeProduto, lances.get(0).getNome());
	}

	@Test
	public void disputa() throws Exception {
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 1000));
		mercado.daLance(nomeProduto, cpfUsuario, valorDoLance);
		mercado.daLance(nomeProduto, cpfLeiloadorProduto, valorDoLance + 1);
		Thread.sleep(2000);
		assertEquals(0, mercado.getProdutosEmLeilao().size());
		assertEquals(0, mercado.getProdutosVencidosENaoVendidos().size());
		List<? extends IVendido> vendidos = mercado.getProdutosVendidos();
		assertEquals(1, vendidos.size());
		assertEquals(nomeProduto, vendidos.get(0).getNome());
		assertEquals(cpfLeiloadorProduto, vendidos.get(0).getCpfComprador());
		assertEquals(new Double(valorDoLance + 1), vendidos.get(0)
				.getValorUltimoLance());

		List<? extends ILeiloavel> lances = mercado
				.getProdutosQueDeuLance(cpfUsuario);
		assertEquals(1, lances.size());
		assertEquals(nomeProduto, lances.get(0).getNome());

		List<? extends ILeiloavel> lancesLeiloador = mercado
				.getProdutosQueDeuLance(cpfLeiloadorProduto);
		assertEquals(1, lancesLeiloador.size());
		assertEquals(nomeProduto, lancesLeiloador.get(0).getNome());
		List<? extends IVendido> comprados = mercado.getUsuarioPor(
				cpfLeiloadorProduto).getBensComprados();
		assertEquals(1, comprados.size());
		assertEquals(nomeProduto, comprados.get(0).getNome());
		assertEquals(0, mercado.getUsuarioPor(cpfUsuario).getBensComprados()
				.size());

	}

	@Test
	public void disputaComValorMenor() throws Exception {
		mercado.cadastrarProduto(nomeProduto, descricaoProduto,
				valorMinimoProduto, cpfLeiloadorProduto,
				new Date(System.currentTimeMillis() + 1000));
		mercado.daLance(nomeProduto, cpfUsuario, valorDoLance);
		mercado.daLance(nomeProduto, cpfLeiloadorProduto, valorDoLance + 2);
		try {
			mercado.daLance(nomeProduto, cpfLeiloadorProduto, valorDoLance + 1);
			fail("Valor Menor");
		} catch (Exception e) {
		}
		Thread.sleep(2000);
		assertEquals(0, mercado.getProdutosEmLeilao().size());
		assertEquals(0, mercado.getProdutosVencidosENaoVendidos().size());
		List<? extends IVendido> vendidos = mercado.getProdutosVendidos();
		assertEquals(1, vendidos.size());
		assertEquals(nomeProduto, vendidos.get(0).getNome());
		assertEquals(cpfLeiloadorProduto, vendidos.get(0).getCpfComprador());
		assertEquals(new Double(valorDoLance + 2), vendidos.get(0)
				.getValorUltimoLance());

		List<? extends ILeiloavel> lances = mercado
				.getProdutosQueDeuLance(cpfUsuario);
		assertEquals(1, lances.size());
		assertEquals(nomeProduto, lances.get(0).getNome());

		List<? extends ILeiloavel> lancesLeiloador = mercado
				.getProdutosQueDeuLance(cpfLeiloadorProduto);
		assertEquals(1, lancesLeiloador.size());
		assertEquals(nomeProduto, lancesLeiloador.get(0).getNome());
		List<? extends IVendido> comprados = mercado.getUsuarioPor(
				cpfLeiloadorProduto).getBensComprados();
		assertEquals(1, comprados.size());
		assertEquals(nomeProduto, comprados.get(0).getNome());
		assertEquals(0, mercado.getUsuarioPor(cpfUsuario).getBensComprados()
				.size());

	}

}