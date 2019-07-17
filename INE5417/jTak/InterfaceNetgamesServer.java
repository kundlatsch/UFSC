import br.ufsc.inf.leobr.cliente.Jogada;
import br.ufsc.inf.leobr.cliente.OuvidorProxy;
import br.ufsc.inf.leobr.cliente.Proxy;
import br.ufsc.inf.leobr.cliente.exception.ArquivoMultiplayerException;
import br.ufsc.inf.leobr.cliente.exception.JahConectadoException;
import br.ufsc.inf.leobr.cliente.exception.NaoConectadoException;
import br.ufsc.inf.leobr.cliente.exception.NaoPossivelConectarException;
import br.ufsc.inf.leobr.cliente.Jogada;
import br.ufsc.inf.leobr.cliente.OuvidorProxy;
import br.ufsc.inf.leobr.cliente.Proxy;
import br.ufsc.inf.leobr.cliente.exception.NaoConectadoException;
import br.ufsc.inf.leobr.cliente.exception.NaoJogandoException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class InterfaceNetgamesServer implements OuvidorProxy {
	
	private static final long serialVersionUID = 1L;
	protected Proxy proxy;
	protected Controlador controlador;

	public InterfaceNetgamesServer(Controlador controlador) {
		super();
		this.controlador = controlador;
		this.proxy = Proxy.getInstance();
		proxy.addOuvinte(this);
	}
	
	public String conectar(String servidor, String nome) {
			try {
				proxy.conectar(servidor, nome);
			} catch (JahConectadoException e) {
				// TODO Auto-generated catch block
				//this.desconectar();
				return "Você já está conectado!";
			} catch (NaoPossivelConectarException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return "Não foi possível conectar-se ao servidor!";
			} catch (ArquivoMultiplayerException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return "Arquivo de propriedades não encontrado!";
			}
			return "Conexão realizada com sucesso!";
		
	}

	public String desconectar() {
			try {
				proxy.desconectar();
			} catch (NaoConectadoException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return "Você não está conectado!";
			}
			return "Você se desconectou do servidor com sucesso!";
	}

	public String iniciarPartida() {
		try {
			proxy.iniciarPartida(new Integer(2)); // supondo 2 jogadores, o que pode ser alterado
		} catch (NaoConectadoException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "Falha ao tentar enviar solicitação de começo de partida ao servidor!";
		}
		try {
            String aux = proxy.obterNomeAdversario(new Integer(2));
        } catch (IndexOutOfBoundsException e) {
            return "Número insuficiente de jogadores para começar uma partida!";
        }
		return "Partida Iniciada com sucesso!";
	}

	public int verificarJogadores() {
		return proxy.obterNomeAdversarios().size();
	}

	@Override
	public void iniciarNovaPartida(Integer posicao) {
		// TODO Auto-generated method stub
		System.out.println("Partida inciada");
	}

	@Override
	public void finalizarPartidaComErro(String message) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void receberMensagem(String msg) {
		// TODO Auto-generated method stub
	}

	@Override
    public void receberJogada(Jogada jogada) {
        Lance lance = (Lance) jogada;
        try {
            controlador.receberJogada(lance);
        } catch (InterruptedException ex) {
            Logger.getLogger(InterfaceNetgamesServer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void enviarJogada(Jogada jogada) {
        try {
            proxy.enviaJogada(jogada);
        } catch (NaoJogandoException e) {
            //controlador.desconectarAdvSaiu();
        }
    }

	@Override
	public void tratarConexaoPerdida() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void tratarPartidaNaoIniciada(String message) {
		// TODO Auto-generated method stub
	}


}
