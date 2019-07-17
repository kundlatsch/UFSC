import java.io.IOException;
import java.util.Stack;
import java.util.ArrayList;
import br.ufsc.inf.leobr.cliente.Jogada;

public class Controlador {
	
	protected boolean conectado = false;
	protected boolean partidaAndamento = false;
	protected InterfaceNetgamesServer ngServer;
	protected Tabuleiro tabuleiro;
	protected boolean vez;
	protected InterfaceJogador interfaceJogador;
	protected int tamanhoTabuleiro;
	protected boolean primeiraJogada = true;
	protected int numeroPecas = 20;
	protected boolean reiAtivo = false;
	protected int cor = 20; // cor 10 = azul, cor 20 = vermelho
	protected int vencedor = 0; // 0 = nao há, 1 = jogador local, 2 = outro jogador
	
	
	public Controlador(InterfaceJogador interfaceJogador, int tamanhoTabuleiro) {
		this.interfaceJogador = interfaceJogador;
		this.tamanhoTabuleiro = tamanhoTabuleiro;
		ngServer = new InterfaceNetgamesServer(this);
		tabuleiro = new Tabuleiro(tamanhoTabuleiro);
		vez = true;

	}

	public void enviarJogada(Lance lance) throws InterruptedException {
		if (partidaAndamento) {
	        vez = false;
	    	lance.setCor(cor);
	        tabuleiro.inserirPeca(lance.getA(), lance.getB(), lance.getTipoJogada(), cor);

	        if(lance.getTipoJogada() == 2) {
	        	if(reiAtivo) {
	        		interfaceJogador.notificar("Erro: você já jogou o rei, faça outra jogada");
	        	}
	        	else {
	        		reiAtivo = true;
	        		interfaceJogador.esconderRei();
	        		ngServer.enviarJogada(lance);
	        	}
	        }
	        else if(numeroPecas > 0) {
	        	ngServer.enviarJogada(lance);
	        	numeroPecas = numeroPecas - 1;
	        }
	        else {
	        	interfaceJogador.notificar("Você não tem mais peças para colocar, mova alguma que já está no tabuleiro!");
	        }

	        if(tabuleiro.verificaVitoria(this.cor)) {
	        	interfaceJogador.alternarTurno(4);
	        	vencedor = 1;
	        }
	        else {
	        	interfaceJogador.alternarTurno(2);
    		}
    	}
    	else {
    		System.out.println("A partida não está em andamento");
    	}
    }

    public void moverPecas(ArrayList pecas) {
    	for(int i = 0; i < pecas.size(); i++) {
    		
    		Lance lance = (Lance) pecas.get(i);
    		
    		tabuleiro.inserirPeca(lance.getA(), lance.getB(), lance.getTipoJogada(), lance.getCor());
    		if(i + 1 == pecas.size())
    			lance.setMovimentando(false);
    		else
    			lance.setMovimentando(true);
    		ngServer.enviarJogada(lance);
	       	
	       	
       	}
       	interfaceJogador.alternarTurno(2);
       	System.out.println("saindo");
       	if(tabuleiro.verificaVitoria(this.cor)) {
       		System.out.println("ue");
        	interfaceJogador.alternarTurno(4);
       		vencedor = 1;
       	}
       	vez = false;
    }

    public void receberJogada(Lance lance) throws InterruptedException {
    	if(!lance.getDesconectar()) {
	    	if(!lance.getMovimentando()) {
		    	if(primeiraJogada) {
		    		primeiraJogada = false;
		    		partidaAndamento = true;
		    	}
		    	tabuleiro.inserirPeca(lance.getA(), lance.getB(), lance.getTipoJogada(), lance.getCor());
		        interfaceJogador.atualizarTabuleiro(lance);

		        vez = true;

		    	if(tabuleiro.verificaVitoria(getCorAdversario())) {
		        	vencedor = 2;
		    		vez = false;
		    		interfaceJogador.alternarTurno(5);
		    	}
		        
		        interfaceJogador.alternarTurno(1);
		        if(lance.getMovA() != -1) {
		        	interfaceJogador.atualizarTopo(lance);
		        }
	    	}
	    	else {
	    		tabuleiro.inserirPeca(lance.getA(), lance.getB(), lance.getTipoJogada(), lance.getCor());
	    		interfaceJogador.atualizarTabuleiro(lance);
	    	}
    	}
    	else {
    		this.reiniciar();
    		System.out.println("desconectando...");
    		//System.out.println(ngServer.desconectar());
    		System.out.println("desconectado");
    	}
    }


    public int getCorAdversario() {
    	if(cor == 10) {
    		return 20;
    	}
    	else
    		return 10;
    }

    

	public String conectar(String ip, String nome) {
		return ngServer.conectar(ip, nome);
	}

	public void reiniciar() {
		System.out.println("REINICIAR");
		partidaAndamento = false;
		vez = true;
		reiAtivo = false;
		cor = 20;
		vencedor = 0;
		primeiraJogada = true;
		conectado = false;
		tabuleiro = new Tabuleiro(tamanhoTabuleiro);
		numeroPecas = 20;
		interfaceJogador.limparTabuleiro();
		interfaceJogador.alternarTurno(3);
		ngServer.desconectar();
	}

	public String desconectar() {
		if(partidaAndamento) {
			switch(vencedor) {
				case 0:
					interfaceJogador.notificar("Não houve vencedores.");
					break;
				case 1:
					interfaceJogador.notificar("Você venceu!");
					break;
				case 2:
					interfaceJogador.notificar("Você perdeu!");
					break;
				default:
					interfaceJogador.notificar("Encerrando partida...");
					break;
			}
			Lance lance = new Lance();
			lance.setDesconectar(true);
			ngServer.enviarJogada(lance);	
			this.reiniciar();
			return "Você se desconectou!";
		}
		else if(conectado) {
			this.reiniciar();
			return "Você se desconectou!";
		}
		else {
			return "Você não está conectado!";
		}
		
		
	}

	public String iniciarPartida() {
		String mensagem = ngServer.iniciarPartida();
			if(mensagem.equals("Partida Iniciada com sucesso!")) {
			cor = 10;
			partidaAndamento = true;
			interfaceJogador.alternarTurno(1);
		}
		return mensagem;
	}

	public void definirConectado(boolean valor) {
		conectado = valor;
	}
	
	public boolean estaConectado() {
		return conectado;
	}
	
	public void definirPartidaAndamento(boolean valor) {
		partidaAndamento = valor;
	}
	
	public boolean informarPartidaAndamento() {
		return partidaAndamento;
	}
	
	public boolean permitidoConectar() {
		return !conectado;
		//todo logic
	}
	
	public boolean permitidoDesconectar() {
		return conectado;
		//todo logic
	}

	public boolean permitidoIniciarPartida() {
		return !partidaAndamento;
		//todo logic
	}

	public boolean permitidoJogar() {
		if(vencedor == 2) {
			interfaceJogador.alternarTurno(5);
		}
		return vez;
	}

	public int getTamanhoTabuleiro() {
		return tamanhoTabuleiro;
	}

	public Stack getPilhaPecasCasa(int a, int b) {
		return tabuleiro.getPilha(a, b);
	}

	public int getNumeroPecas() {
		return numeroPecas;
	}

	public int getCor() {
		return cor;
	}
	
	
}
