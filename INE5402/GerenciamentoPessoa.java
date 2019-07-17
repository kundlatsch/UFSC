public class GerenciamentoPessoa {
	private static int numPessoasMax = 10;
	private Pessoa[] pessoas = new Pessoa[numPessoasMax];
	private int eventosMax = Controle.getNumEventosMax();
	private Interface ui = new Interface();
	private Evento[] eventosP = Controle.getEventos();

	public void inserirPessoa(int id, String nome, int cpf, int idade,
			String cidade, Evento[] participacoes) {
		pessoas[id] = new Pessoa(nome, cpf, idade, cidade, participacoes);
	}

	public Evento[] solicitaParticipacoes() {
		Evento[] retorno = new Evento[eventosMax];
		boolean controle = true;
		int tryMax = 0;
		int opcao = ui
				.solicitaInt("Voce deseja registrar eventos nessa pessoa?\n1) Sim\n2) Nao");
		if (opcao == 1) {
			while (controle && tryMax < eventosMax) {
				if (Controle.mostrarNomeData().equals("")) {
					ui.mostrarMensagem("Nenhum Evento Encontrado");
				} else {
					retorno[tryMax] = eventosP[ui.solicitaInt(Controle
							.mostrarNomeData()
							+ "\nInsira o evento que a pessoa participou: ")];
					if (retorno[tryMax] == null)
						ui.mostrarMensagem("Evento Inexistente");
				}
				int opcaoI = ui
						.solicitaInt("Inserir mais um evento?\n1) Sim\n2) Nao");
				if (opcaoI != 1)
					controle = false;
				tryMax++;

			}
		} else if (opcao == 2)
			return null;
		else
			ui.mostrarMensagem("Opcao Invalida!");
		return retorno;
	}

	public void excluirPessoa(int id) {
		pessoas[id] = null;
	}

	public int consultaPorEvento(String nome, int[] data) {
		int numParticipantes = 0;
		for (int i = 0; i < pessoas.length; i++) {
			for (int j = 0; j < eventosMax; j++) {
				if (pessoas[i] != null) {
					if (pessoas[i].getParticipacoes() != null) {
						if (pessoas[i].getParticipacoes()[j] != null) {
							if (pessoas[i].getParticipacoes()[j] != null) {
								if ((pessoas[i].getParticipacoes()[j].getNome()
										.equals(nome))) {
									numParticipantes++;
								}
							}
						}
					}
				}
			}
		}
		return numParticipantes;
	}

	public int consultaPorData(int[] dataBusca) {
		int numParticipantes = 0;
		for (int i = 0; i < pessoas.length; i++) {
			for (int j = 0; j < eventosMax; j++) {
				if (pessoas[i] != null) {
					if (pessoas[i].getParticipacoes() != null) {
						if (pessoas[i].getParticipacoes()[j] != null) {
							if (pessoas[i].getParticipacoes()[j].getData()[0] == dataBusca[0]
									&& pessoas[i].getParticipacoes()[j]
											.getData()[1] == dataBusca[1]
									&& pessoas[i].getParticipacoes()[j]
											.getData()[2] == dataBusca[2]) {
								numParticipantes++;
							}
						}
					}
				}
			}
		}
		return numParticipantes;
	}

	public String consultaPorNome(String pessoa, String evento) {
		Pessoa[] retorno = new Pessoa[numPessoasMax];
		int k = 0;
		for (int i = 0; i < pessoas.length; i++) {
			for (int j = 0; j < eventosMax; j++) {
				if (pessoas[i] != null) {
					if (pessoas[i] != null) {

						if (pessoas[i].getParticipacoes() != null) {
							if (pessoas[i].getNome() != null) {
								if (pessoas[i].getParticipacoes()[j] != null) {
									if (pessoas[i].getParticipacoes()[j]
											.getNome().equals(evento)
											&& pessoas[i].getNome().equals(
													pessoa)) {
										retorno[k] = pessoas[i];
									}
								}
							}
						}
					}
				}
			}
		}

		return mostrarNomeCpf(retorno);
	}

	public String mostrarNomeCpf(Pessoa[] eventos) {
		String mensagem = "";
		for (int i = 0; i < pessoas.length; i++) {
			if (pessoas[i] != null)
				mensagem += (i + ") " + pessoas[i].getNome() + " "
						+ pessoas[i].getCpf() + "\n");
		}
		return mensagem;
	}

	public Pessoa[] getPessoas() {
		return pessoas;
	}

	public static int getNumPessoasMax() {
		return numPessoasMax;
	}
}
