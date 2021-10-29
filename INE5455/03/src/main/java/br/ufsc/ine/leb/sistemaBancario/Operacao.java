package br.ufsc.ine.leb.sistemaBancario;

public class Operacao {

	private EstadosDeOperacao estado;

	public Operacao(EstadosDeOperacao estado, Transacao... transacoes) {
		this.estado = estado;
	}

	public EstadosDeOperacao obterEstado() {
		return estado;
	}

}
