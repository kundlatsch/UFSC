import java.util.ArrayList;
import java.util.Stack;
import java.util.Collections;

public class InterfaceJogador {
	
	protected Controlador domProblema;
	protected String serverIP;
	protected String nomeJogador;
	protected Board board;

	public InterfaceJogador(Board board, int tamanhoTabuleiro) {
		this.board = board;
		domProblema = new Controlador(this, tamanhoTabuleiro);
		serverIP = "localhost";
		nomeJogador = "jogador";
	}

	public String conectar() {
		String mensagem = "Falha ao conectar-se ao servidor!";
		boolean permitido = domProblema.permitidoConectar();
		if (permitido) {
			mensagem = domProblema.conectar(this.serverIP, this.nomeJogador);
			if (mensagem.equals("Conexão realizada com sucesso!")) {
				domProblema.definirConectado(true);
			}
		}
		return mensagem;
	}

	public void enviarJogada(int a, int b, int tipoJogada) throws InterruptedException {
		Lance lance = new Lance(a, b, tipoJogada);
		domProblema.enviarJogada(lance);
	}

	public void enviarJogada(Lance lance) {
		try {
			domProblema.enviarJogada(lance);
		}
		catch(InterruptedException e) {
			notificar("Erro ao receber lance da movimentação.");
		}
	}

	public void moverPecas(ArrayList lances) {
		domProblema.moverPecas(lances);
	}


	
	public String desconectar() {
		String mensagem = "Não foi possível desconectar-se do servidor";
		boolean permitido = domProblema.permitidoDesconectar();
		if (permitido) {
			mensagem = domProblema.desconectar();
			if (mensagem.equals("Você se desconectou do servidor com sucesso!")) {
				domProblema.definirConectado(false);
			}
		}
		return mensagem;
	}
	
	public String comecarPartida() {
		String mensagem = "Falha ao começar partida!";
		boolean permitido = domProblema.permitidoIniciarPartida();
		if (permitido) {
			mensagem = domProblema.iniciarPartida();
		}
		return mensagem;
	}

	public void atualizarTabuleiro(Lance lance) {
		board.atualizarCasa(lance.getA(), lance.getB(), lance.getTipoJogada(), lance.getCor());
	}

	public void notificar(String mensagem) {
		board.alertar(mensagem);
	}


	public String getIP() {
		return this.serverIP;
	}

	public void setIP(String serverIP) {
		this.serverIP = serverIP;
	}


	public String getNomeJogador() {
		return this.nomeJogador;
	}

	public void setNomeJogador(String nomeJogador) {
		this.nomeJogador = nomeJogador;
	}

	public int getTamanhoTabuleiro() {
		return domProblema.getTamanhoTabuleiro();
	}

	public boolean meuTurno() {
		return domProblema.permitidoJogar();
	}

	public int getCor() {
		return domProblema.getCor();
	}

	public String podeJogar() {
		String retorno = "";
		//return (domProblema.permitidoDesconectar() && !domProblema.permitidoIniciarPartida());
		if(domProblema.permitidoConectar())
			retorno = "Você não está conectado no servidor";
		else if(domProblema.permitidoIniciarPartida())
			retorno = "A partida ainda não iniciou";
		return retorno;
	}

	public int getTopoPilhaCasa(int a, int b) {
		Stack stack = domProblema.getPilhaPecasCasa(a, b);
		ArrayList<Integer> listaTipos = new ArrayList(stack);
		Collections.reverse(listaTipos);
		try {
			return (Integer) listaTipos.get(0);
		}
		catch (IndexOutOfBoundsException e) {
			return -1;
		}
	}

	public void removerDoTopo(int a, int b, int n) {
		Stack stack = domProblema.getPilhaPecasCasa(a, b);
		for(int i = 0; i < n; i++) {
			stack.pop();
		}
	}

	public ArrayList<String> getListaPecasCasa(int a, int b) {
		Stack stack = domProblema.getPilhaPecasCasa(a, b);
		ArrayList<Integer> listaTipos = new ArrayList(stack);
		Collections.reverse(listaTipos);
		ArrayList<String> listaStrings = new ArrayList();
		for(int peca : listaTipos) {
			if(peca == 10)
				listaStrings.add("Estrada Azul");
			else if(peca == 11)
				listaStrings.add("Parede Azul");
			else if(peca == 12)
				listaStrings.add("Rei Azul");
			else if(peca == 20)
				listaStrings.add("Estrada Vermelha");
			else if(peca == 21)
				listaStrings.add("Parede Vermelha");
			else if(peca == 22)
				listaStrings.add("Rei Vermelha");
				
		}
		return listaStrings;
	}

	public Stack getSubPilha(int a, int b, int numeroPecas) {
		Stack stack = domProblema.getPilhaPecasCasa(a, b);
		ArrayList<Integer> listaTipos = new ArrayList(stack);
		Collections.reverse(listaTipos);
		Stack retorno = new Stack();
		for(int i = 0; i < numeroPecas; i++) {
			retorno.push(listaTipos.get(i));
		}
		return retorno;
	}

	public int getNumeroPecas() {
		return domProblema.getNumeroPecas();
	}

	public void esconderRei() {
		board.hideKing();
	}

	public void alternarTurno(int tipo) {
		board.setTurnText(tipo);
	}

	public boolean verificarCor(int peca) {
		int cor = domProblema.getCor();
		if(peca == -1) {
    		return true;
    	}
		else if(cor == 10) {
    		if(peca < 19)
    			return true;
    		else
    			return false;
	    }
    	else if(cor == 20) {
    		if(peca > 19)
    			return true;
    		else
    			return false;
    	}
    	return false;

	}

	public void limparTabuleiro() {
		board.limparTabuleiro();
		board.atualizarNumeroPecas();
	}

	public void atualizarTopo(Lance lanceAt) {
		int a = lanceAt.getMovA();
		int b = lanceAt.getMovB();
		removerDoTopo(a, b, lanceAt.getTamMov());
		int pecaAt = getTopoPilhaCasa(a, b);
		if(pecaAt != -1) {
			int tipoJogadaL = 0;
			if(pecaAt == 10 || pecaAt == 20)
				tipoJogadaL = 0;
			else if (pecaAt == 11 || pecaAt == 21)
				tipoJogadaL = 1;
			else
				tipoJogadaL = 2;

			int corAt = 0;
			if(pecaAt < 15)
				corAt = 10;
			else
				corAt = 20;
			
			board.atualizarCasa(a, b, tipoJogadaL, corAt);
		}
		else {
			
			board.limparCasa(a, b);
		}
	}

}
