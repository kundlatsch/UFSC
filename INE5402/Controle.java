import java.util.Arrays;

public class Controle {

	private static Interface ui = new Interface();
	public static int numEventosMax = 10;
	private static Evento[] eventos = new Evento[numEventosMax];

	public static Evento[] getEventos() {
		return eventos;
	}

	public static int getNumEventosMax() {
		return numEventosMax;
	}

	public static Evento[] consultaEvento(int opcao, String busca) {
		Evento[] retorno = new Evento[numEventosMax];
		int j = 0;
		switch (opcao) {
		case 1:
			for (int i = 0; i < eventos.length; i++) {
				if (eventos[i] != null) {
					if (busca.equals(eventos[i].getNome())) {
						retorno[j] = eventos[i];
						j++;
					}
				}
			}
			break;

		case 2:
			for (int i = 0; i < eventos.length; i++) {
				if (eventos[i] != null) {
					if (busca.equals(eventos[i].getAutor())) {
						retorno[j] = eventos[i];
						j++;
					}
				}
			}
			break;

		case 3:
			for (int i = 0; i < eventos.length; i++) {
				if (eventos[i] != null) {
					if (Double.parseDouble(busca) == eventos[i].getPreco()) {
						retorno[j] = eventos[i];
						j++;
					}
				}
			}
			break;

		// Para consultar por data usar o metodo consultaPorData

		case 4:

			for (int i = 0; i < eventos.length; i++) {
				if (eventos[i] != null) {
					if (busca.equals(eventos[i].getLocal())) {
						retorno[j] = eventos[i];
						j++;
					}
				}
			}
			break;

		default:
			System.out.println("Opcao Invalida!");
			return null;

		}
		return retorno;

	}

	public static Evento[] consultaDataEspecifica(int dia, int mes, int ano) {
		Evento[] retorno = new Evento[numEventosMax];
		int j = 0;
		int[] consulta = new int[3];

		for (int i = 0; i < eventos.length; i++) {
			if (eventos[i] != null) {
				consulta = eventos[i].getData();
				if (consulta[0] == dia && consulta[1] == mes
						&& consulta[2] == ano) {
					retorno[j] = eventos[i];
					j++;
				}
			}
		}
		return retorno;
	}

	public static Evento[] consultaDataPeriodo(int dia1, int mes1, int ano1,
			int dia2, int mes2, int ano2) {
		Evento[] retorno = new Evento[numEventosMax];
		int j = 0;
		int[] consulta = new int[3];

		for (int i = 0; i < eventos.length; i++) {
			if (eventos[i] != null) {
				consulta = eventos[i].getData();
				if (consulta[2] >= ano1 && consulta[2] <= ano2) {
					System.out.println("ano");
					if ((consulta[1] >= mes1 && consulta[1] <= mes2)
							|| (ano1 != ano2 && consulta[1] <= mes2)) {
						System.out.println("mes");
						if ((consulta[0] >= dia1 && consulta[0] <= dia2)
								|| (mes1 != mes2 && consulta[0] <= dia2)
								|| (ano1 != ano2 && (consulta[0] <= dia2 || consulta[0] >= dia2))) {
							retorno[j] = eventos[i];
							j++;
						}
					}
				}
			}
		}
		return retorno;
	}

	public static int[] modeloData(int dia, int mes, int ano) {
		int[] retorno = new int[3];
		if (dia > 31 || mes > 12 || ano < 1950){
			return null;
		}
		retorno[0] = dia;
		retorno[1] = mes;
		retorno[2] = ano;
		return retorno;
	}

	public static String mostrarNomeData() {
		String mensagem = "";
		// String[] mensagem = new String[numEventosMax];
		for (int i = 0; i < eventos.length; i++) {
			if (eventos[i] != null)
				mensagem += (i + ") " + eventos[i].getNome() + " "
						+ Arrays.toString(eventos[i].getData()) + "\n");
		}
		return mensagem;
		// return Arrays.toString(mensagem);
	}

	public static String mostrarEvento(int p, Evento[] evento) {
		return ("Nome: " + evento[p].getNome() + "\nAutor: "
				+ evento[p].getAutor() + "\nPreco: " + evento[p].getPreco()
				+ "\nData" + Arrays.toString(evento[p].getData()) + "\nLocal: "
				+ evento[p].getLocal() + "\n");

	}

	public static void main(String[] args) {
		int[] data = new int[3];
		GerenciamentoPessoa rh = new GerenciamentoPessoa();
		int indiceEventos = 0;
		boolean repetir = true;
		ui.mostrarMensagem("Para melhor uso do programa, entre apenas o tipo de dados esperado!");
		while (repetir) {
			switch (ui
					.solicitaInt("-----------------------\nGerenciador\n-----------------------\nOpcoes:\n1) Gerenciar Eventos\n2) Gerenciar Pessoas\n3) Sair do Programa\nEscolha uma Opcao:")) {
			case 1:
				boolean gerenciarEventos = true;
				while (gerenciarEventos) {
					// ui.mostrarMensagem("n1) Criar Evento\n2) Editar Evento\n3) Excluir Evento\n4) Consultar Evento");
					switch (ui
							.solicitaInt("1) Criar Evento\n2) Editar Evento\n3) Excluir Evento\n4) Consultar Evento\n5) Voltar\nEscolha uma Opcao: ")) {
					case 1: // Criar Evento
						if (indiceEventos < numEventosMax) {
							String solicitaNome = ui
									.solicitaString("Nome do Evento: ");
							String solicitaAutor = ui.solicitaString("Autor: ");
							double solicitaPreco = ui.solicitaDouble("Preco: ");
							int[] solicitaData = modeloData(
									ui.solicitaInt("Data do Evento\nDia: "),
									ui.solicitaInt("Data do Evento\nMes: "),
									ui.solicitaInt("Data do Evento\nAno: "));
							if (solicitaData == null) {
								ui.mostrarMensagem("Data Invalida");
								break;
							}
							String solicitaLocal = ui.solicitaString("Local: ");
							if (ui.solicitaInt("O evento e promocional?\n1) Sim\n2) Nao") == 1) {
								int solicitaDesconto = ui
										.solicitaInt("Desconto em porcentagem: ");
								int solicitaCodigo = ui
										.solicitaInt("Codigo promocional em numeros decimais: ");
								if (solicitaData != null) {
									eventos[indiceEventos] = new EventoPromocional(
											solicitaNome, solicitaAutor,
											solicitaPreco, solicitaData,
											solicitaLocal, solicitaCodigo,
											solicitaDesconto);
								}
							} else {
								if (solicitaData != null) {
									eventos[indiceEventos] = new Evento(
											solicitaNome, solicitaAutor,
											solicitaPreco, solicitaData,
											solicitaLocal);
								}
							}
							boolean verificaIgualdade = false;
							if (indiceEventos > 0) {
								for (int i = 0; i < indiceEventos; i++) {
									if (solicitaNome.equals(eventos[i]
											.getNome())
											&& solicitaAutor.equals(eventos[i]
													.getAutor())
											&& solicitaData[0] == eventos[i]
													.getData()[0]
											&& solicitaData[1] == eventos[i]
													.getData()[1]
											&& solicitaData[2] == eventos[i]
													.getData()[2]) {
										verificaIgualdade = true;
										break;
									}
								}
							}

							if (verificaIgualdade) {
								eventos[indiceEventos].excluirEvento(
										indiceEventos, eventos);
								ui.mostrarMensagem("Evento ja existente");
								
							} else
								indiceEventos++;
						} else {
							ui.mostrarMensagem("Numero de Eventos Maximo Gravado!");
						}
						break;
					case 2: // Editar Evento
						switch (ui
								.solicitaInt("---------\nEdicao\n---------\n1) Nome\n2) Autor\n3) Preco\n4) Data\n5) Local\nEscolha uma Opcao: ")) {
						case 1: // Editar Nome
							int a = (ui.solicitaInt(mostrarNomeData()
									+ "\nEscolha um Evento: "));
							if (eventos[a].getNome() != null)
								eventos[a]
										.setNome(ui
												.solicitaString("Insira o Novo Nome: "));
							else
								ui.mostrarMensagem("Opcao Invalida!");
							break;
						case 2: // Editar Autor
							int b = (ui.solicitaInt(mostrarNomeData()
									+ "\nEscolha um Evento: "));
							if (eventos[b].getNome() != null)
								eventos[b]
										.setAutor(ui
												.solicitaString("Insira o Novo Autor: "));
							else
								ui.mostrarMensagem("Opcao Invalida!");
							break;
						case 3: // Editar Preco
							int c = (ui.solicitaInt(mostrarNomeData()
									+ "\nEscolha um Evento: "));
							if (eventos[c].getNome() != null)
								eventos[c]
										.setPreco(ui
												.solicitaDouble("Insira o Novo Preco: "));
							else
								ui.mostrarMensagem("Opcao Invalida!");
							break;
						case 4: // Editar Data
							int d = (ui.solicitaInt(mostrarNomeData()
									+ "\nEscolha um Evento: "));
							if (eventos[d].getNome() != null)
								eventos[d].setData(modeloData(
										ui.solicitaInt("Insira o Dia: "),
										ui.solicitaInt("Insira o Mes: "),
										ui.solicitaInt("Insira o Ano: ")));
							else
								ui.mostrarMensagem("Opcao Invalida!");
							break;

						case 5: // Editar Autor
							int e = (ui.solicitaInt(mostrarNomeData()
									+ "\nEscolha um Evento: "));
							if (eventos[e].getNome() != null)
								eventos[e]
										.setLocal(ui
												.solicitaString("Insira o Novo Local: "));
							else
								ui.mostrarMensagem("Opcao Invalida!");
							break;
						}
						break;
					case 3: // Excluir Evento
						int a = (ui.solicitaInt(mostrarNomeData()
								+ "\nEscolha uma Opcao: "));
						if (eventos[a].getNome() != null) {
							eventos[a].excluirEvento(a, eventos);
							ui.mostrarMensagem("Evento Excluido com Sucesso");
						} else
							ui.mostrarMensagem("Opcao Invalida!");
						break;
					case 4: // Consultar Evento
						switch (ui
								.solicitaInt("Consulta por:\n1) Data Especifica\n2) Data por Periodo\n3) Nome\n4) Autor\n5) Preco\n6) Local\n7) Voltar\nEscolha uma Opcao: ")) {
						case 1: // Consulta por data especifica
							Evento[] dataEspecifica = consultaDataEspecifica(
									ui.solicitaInt("Dia: "),
									ui.solicitaInt("Mes: "),
									ui.solicitaInt("Ano: "));
							boolean data1Ctr = true;
							for (int i = 0; i < dataEspecifica.length; i++) {
								if (dataEspecifica[i] != null) {
									ui.mostrarMensagem(mostrarEvento(i,
											dataEspecifica));
									data1Ctr = false;
								}
							}

							if (data1Ctr)
								ui.mostrarMensagem("Nenhum evento encontrado");
							break;
						case 2: // Consulta por periodo
							Evento[] periodo = consultaDataPeriodo(
									ui.solicitaInt("\nData Inicial\nDia: "),
									ui.solicitaInt("\nData Inicial\nMes: "),
									ui.solicitaInt("\nData Inicial\nAno: "),
									ui.solicitaInt("\nData Final\nDia: "),
									ui.solicitaInt("\nData Final\nMes: "),
									ui.solicitaInt("\nData Final\nAno: "));
							boolean data2Ctr = true; // true significa nao
														// encontrar
														// nenhum
							// evento

							for (int i = 0; i < periodo.length; i++) {
								if (periodo[i] != null) {
									ui.mostrarMensagem(mostrarEvento(i, periodo));
									data2Ctr = false;
								}
							}
							if (data2Ctr)
								ui.mostrarMensagem("Nenhum evento encontrado");
							break;

						case 3: // Consulta por nome
							Evento[] nome = consultaEvento(1,
									ui.solicitaString("Pesquisar Nome: "));
							boolean nomeCtr = true;

							for (int i = 0; i < nome.length; i++) {
								if (nome[i] != null) {
									ui.mostrarMensagem(mostrarEvento(i, nome));
									nomeCtr = false;
								}
							}
							if (nomeCtr)
								ui.mostrarMensagem("Nenhum evento encontrado");
							break;

						case 4: // Consulta por autor
							Evento[] autor = consultaEvento(2,
									ui.solicitaString("Pesquisar Autor: "));
							boolean autorCtr = true; // true significa nao
														// encontrar
							// nenhum
							// evento

							for (int i = 0; i < autor.length; i++) {
								if (autor[i] != null) {
									ui.mostrarMensagem(mostrarEvento(i, autor));
									autorCtr = false;
								}
							}
							if (autorCtr)
								ui.mostrarMensagem("Nenhum evento encontrado");
							break;

						case 5: // Consulta por preco
							Evento[] preco = consultaEvento(3,
									ui.solicitaString("Pesquisar por Preco: "));
							boolean precoCtr = true; // true significa nao
														// encontrar
							// nenhum
							// evento

							for (int i = 0; i < preco.length; i++) {
								if (preco[i] != null) {
									ui.mostrarMensagem(mostrarEvento(i, preco));
									precoCtr = false;
								}
							}
							if (precoCtr)
								ui.mostrarMensagem("Nenhum evento encontrado");
							break;

						case 6: // Consulta por Local
							Evento[] local = consultaEvento(4,
									ui.solicitaString("Pesquisar por Local: "));
							boolean localCtr = true; // true significa nao
														// encontrar
							// nenhum
							// evento

							for (int i = 0; i < local.length; i++) {
								if (local[i] != null) {
									ui.mostrarMensagem(mostrarEvento(i, local));
									localCtr = false;
								}
							}
							if (localCtr)
								ui.mostrarMensagem("Nenhum evento encontrado");
							break;

						}
						break;
					case 5:
						gerenciarEventos = false;
						break;
					default:
						ui.mostrarMensagem("Opcao Invalida!");

					}
				}
				break;

			case 2:
				int indicePessoas = 0;
				boolean gerenciarPessoas = true;
				while (gerenciarPessoas) {

					switch (ui
							.solicitaInt("1) Inserir Pessoa\n2) Excluir Pessoa\n3) Consultar Pessoa\n4) Voltar\nEscolha uma Opcao: ")) {
					case 1: // Inserir Pessoa
						if (indicePessoas < GerenciamentoPessoa
								.getNumPessoasMax()) {
							String solicitaNome = ui.solicitaString("Nome: ");
							int solicitaCpf = ui
									.solicitaInt("CPF (sem pontos ou virgulas)");
							int solicitaIdade = ui.solicitaInt("Idade: ");
							String solicitaCidade = ui
									.solicitaString("Cidade: ");
							Evento[] solicitaParticipacoes = rh
									.solicitaParticipacoes();
							boolean verificaIgualdade = false;
							rh.inserirPessoa(indicePessoas, solicitaNome,
									solicitaCpf, solicitaIdade, solicitaCidade,
									solicitaParticipacoes);
							if (indicePessoas > 0) {
								System.out.println("Alegria");
								for (int i = 0; i < indicePessoas; i++) {
									if (rh.getPessoas()[i].getCpf() == solicitaCpf) {
										verificaIgualdade = true;
										break;
									}

								}
							}
							if (verificaIgualdade) {
								rh.excluirPessoa(indicePessoas);
								ui.mostrarMensagem("Pessoa ja cadastrada");
							} else
								indicePessoas++;

						} else
							ui.mostrarMensagem("Numero de Pessoas Maximo Gravado!");

						break;

					case 2: // Excluir Pessoa
						int a = (ui.solicitaInt(rh.mostrarNomeCpf(rh
								.getPessoas()) + "\nEscolha uma Opcao: "));
						if (rh.getPessoas()[a] != null) {
							if (rh.getPessoas()[a].getNome() != null) {
								rh.excluirPessoa(a);
								ui.mostrarMensagem("Pessoa Excluida com Sucesso");

							}
						} else
							ui.mostrarMensagem("Opcao Invalida!");
						break;
					case 3:
						switch (ui
								.solicitaInt("1)  Numero de Participantes de um Evento\n2) Numero de Participantes de uma Data\n3) Consultar Pessoas em Eventos por Nome\nEscolha uma Opcao: ")) {
						case 1:
							ui.mostrarMensagem("Numero de Participantes: "
									+ rh.consultaPorEvento(
											ui.solicitaString("Nome do Evento: "),
											modeloData(
													ui.solicitaInt("Insira o Dia do Evento: "),
													ui.solicitaInt("Insira o Mes do Evento: "),
													ui.solicitaInt("Insira o Ano do Evento: "))));
							break;
						case 2:
							ui.mostrarMensagem("Numero de Participantes: "
									+ rh.consultaPorData(modeloData(
											ui.solicitaInt("Insira o Dia do Evento: "),
											ui.solicitaInt("Insira o Mes do Evento: "),
											ui.solicitaInt("Insira o Ano do Evento: "))));
							break;
						case 3:
							ui.mostrarMensagem("Participantes: \n"
									+ rh.consultaPorNome(
											ui.solicitaString("Nome Pessoa: "),
											ui.solicitaString("Nome Evento: ")));

							break;
						}
						break;
					case 4:
						gerenciarPessoas = false;
						break;
					default:
						ui.mostrarMensagem("Opcao Invalida!");
					}
				}
				break;
			case 3:

				repetir = false;
				break;
			default:
				ui.mostrarMensagem("Opcao Invalida!");
			}

		}

	}
}