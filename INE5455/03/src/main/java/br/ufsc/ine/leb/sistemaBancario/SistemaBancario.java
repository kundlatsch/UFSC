package br.ufsc.ine.leb.sistemaBancario;

import java.util.LinkedList;
import java.util.List;

public class SistemaBancario {

	private List<Banco> bancos;

	public SistemaBancario() {
		bancos = new LinkedList<>();
	}

	public Banco criarBanco(String nome, Moeda moeda) {
		Banco banco = new Banco(nome, moeda, new Dinheiro(moeda, 0, 0));
		bancos.add(banco);
		return banco;
	}

	public Operacao depositar(Conta conta, Dinheiro quantia) {
		Transacao entrada = new Entrada(conta, quantia);
		EstadosDeOperacao estado = EstadosDeOperacao.SUCESSO;
		if (moedaInvalida(conta, quantia)) {
			entrada = new TransacaoNaoRealizada(entrada);
			estado = EstadosDeOperacao.MOEDA_INVALIDA;
		}
		conta.adicionarTransacao(entrada);
		return new Operacao(estado, entrada);
	}

	public Operacao sacar(Conta conta, Dinheiro quantia) {
		ValorMonetario saldo = conta.calcularSaldo();
		Transacao saida = new Saida(conta, quantia);
		EstadosDeOperacao estado = EstadosDeOperacao.SUCESSO;
		if (saldo.negativo() || saldoFicaraNegativo(saldo, quantia)) {
			saida = new TransacaoNaoRealizada(saida);
			estado = EstadosDeOperacao.SALDO_INSUFICIENTE;
		}
		if (moedaInvalida(conta, quantia)) {
			saida = new TransacaoNaoRealizada(saida);
			estado = EstadosDeOperacao.MOEDA_INVALIDA;
		}
		conta.adicionarTransacao(saida);
		return new Operacao(estado, saida);
	}

	public Operacao transferir(Conta origem, Conta destino, Dinheiro quantia) {
		Transacao saida = new Saida(origem, quantia);
		Transacao entrada = new Entrada(destino, quantia);
		EstadosDeOperacao estado = EstadosDeOperacao.SUCESSO;
		if (moedaInvalida(origem, quantia) || moedaInvalida(destino, quantia)) {
			saida = new TransacaoNaoRealizada(saida);
			entrada = new TransacaoNaoRealizada(entrada);
			estado = EstadosDeOperacao.MOEDA_INVALIDA;
		}
		origem.adicionarTransacao(saida);
		destino.adicionarTransacao(entrada);
		return new Operacao(estado, saida, entrada);
	}

	private Boolean saldoFicaraNegativo(ValorMonetario saldo, Dinheiro quantia) {
		return saldo.obterQuantia().obterQuantiaEmEscala() < quantia.obterQuantiaEmEscala();
	}

	private Boolean moedaInvalida(Conta conta, Dinheiro quantia) {
		Moeda moedaDoBanco = conta.obterAgencia().obterBanco().obterMoeda();
		Moeda moedaDaOperacao = quantia.obterMoeda();
		return !moedaDoBanco.equals(moedaDaOperacao);
	}

	public List<Banco> obterBancos() {
		return bancos;
	}

}
