
public class Evento {
	private String nome;
	private String autor;
	private double preco;
	private int[] data = new int[3];
	private String local;

	public Evento(String nome, String autor, double preco, int[] data,
			String local) {
		this.nome = nome;
		this.autor = autor;
		this.preco = preco;
		this.data[0] = data[0];
		this.data[1] = data[1];
		this.data[2] = data[2];
		this.local = local;
	}

	public void excluirEvento(int numEvento, Evento[] arrayEventos) {
		arrayEventos[numEvento] = null;
	}

	/*public boolean equals(String nome, String autor, double preco, int[] data,
			String local, String nome2, String autor2, double preco2, int[] data2,
			String local2){
		if(nome.equals(nome2)){
			if(autor.equals(autor2)){
				if(preco == preco2){
					if(data == data2){
						if(local.equals(local2)){
							return true;
						}
					}
				}
			}
		}
		return false;
	}*/
	
	public String getNome() {
		return this.nome;
	}

	public String getAutor() {
		return this.autor;
	}

	public double getPreco() {
		return this.preco;
	}

	public int[] getData() {
		return this.data;
	}

	public String getLocal() {
		return this.local;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}

	public void setAutor(String autor) {
		this.autor = autor;
	}

	public void setPreco(double preco) {
		this.preco = preco;
	}

	public void setData(int[] data) {
		this.data = data;
	}

	public void setLocal(String local) {
		this.local = local;
	}
}
