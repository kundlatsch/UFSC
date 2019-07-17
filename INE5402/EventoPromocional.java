public class EventoPromocional extends Evento{
  private int codigoPromocao;
  private double desconto;

  public EventoPromocional(String nome, String autor, double preco, int[] data, String local, int codigoPromocao, double desconto){
    super(nome, autor, preco, data, local);
    this.codigoPromocao = codigoPromocao;
    this.desconto = desconto;
  }

	public int getCodigoPromocao() {
		return this.codigoPromocao;
	}

	public double getDesconto() {
		return this.desconto;
	}

	public void setCodigoPromocao(int codigoPromocao) {
		this.codigoPromocao = codigoPromocao;
	}

	public void setDesconto(double desconto) {
		this.desconto = desconto;
	}

}
