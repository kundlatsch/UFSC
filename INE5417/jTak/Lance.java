import br.ufsc.inf.leobr.cliente.Jogada;

public class Lance implements Jogada {
	
	protected int posA;
	protected int posB;
	protected int tipoJogada;
	protected int cor;
	protected boolean movimentando;
	protected boolean desconectar;
	protected int movA;
	protected int movB;
	protected int tamMov;

	public Lance() {
		this.desconectar = true;
	}

	public Lance(int a, int b, int tipoJogada) {
		posA = a;
		posB = b;
		movA = -1;
		movB = -1;
		this.movimentando = false;
		this.tipoJogada = tipoJogada;
		this.desconectar = false;
	}

	public void setMovimentando(boolean a) {
		movimentando = a;
	}

	public int getTipoJogada() {
		return this.tipoJogada;
	}

	public boolean getMovimentando() {
		return this.movimentando;
	}

	public boolean getDesconectar() {
		return this.desconectar;
	}

	public void setDesconectar(boolean bol) {
		this.desconectar = bol;
	}


	public void setTipoJogada(int tipoJogada) {
		this.tipoJogada = tipoJogada;
	}

	public int getA() {
		return posA;
	}

	public int getCor() {
		return cor;
	}

	public void setCor(int cor) {
		this.cor = cor;
	}


	public int getB() {
		return posB;
	}


	public void setMovA(int a) {
		this.movA = a;
	}

	public int getMovA() {
		return movA;
	}


	public void setMovB(int b) {
		this.movB = b;
	}

	public int getMovB() {
		return movB;
	}

	public void setTamMov(int a) {
		this.tamMov = a;
	}

	public int getTamMov() {
		return tamMov;
	}
}