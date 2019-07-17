import javafx.application.Application;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Orientation;
import javafx.geometry.Pos;

import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.MenuButton;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextInputDialog;
import javafx.scene.control.ToolBar;
import javafx.scene.Cursor;   
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.ImageCursor;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.FlowPane; 
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Sphere;
import javafx.scene.text.Text;

import javafx.stage.Stage;

import javafx.event.*;

import java.util.ArrayList;
import java.util.Stack;

public class Board extends Application {

  	private InterfaceJogador interfaceJogador;
  	private GridPane root;

  	/*
	Tipos de jogada

	-1: default -> código para começar movimentação de peças
  	0: Jogar estrada
  	1: Jogar parede
  	2: Jogar rei
  	3: Mover peças

  	*/

  	private int tipoJogada;
  	private String imagemJogada;
  	private Text piecesCounterText;
    private Text turnText;

    private Scene scene;

    private Button king;

    private ListView<String> stackList;

    // Variáveis de movimentação de peças
    private int contadorPilhaMovimentacao;
    private int contadorReserva;
    private int ultimaCasaA;
    private int ultimaCasaB;
    private int penultimaCasaA;
    private int penultimaCasaB;
    private ArrayList<Lance> lancesMovimentacao;
    private Stack subPilhaMovimentacao;
    private int pecaTopoMov;
    private int movA;
    private int movB;

    private boolean emJogo;
    private boolean inicioTurno;
    private boolean movendo;
    private boolean posicionando;
    private boolean pilhaVazia;
    private boolean jogoEncerrado;

    /* Compile com javac -cp '.:netgames.jar' *.java */
	public static void main(String[] args) {
 		 launch();
 	}

 	@Override
 	public void start(Stage board) throws Exception {
 		imagemJogada = "";
 		tipoJogada = -1;
 		emJogo = false;
      	interfaceJogador = new InterfaceJogador(this, 7);
  		// Instância do painel principal, que engloba os outros painéis
  		BorderPane border = new BorderPane();
  		
  		// Toolbar do topo
  		ToolBar toolbar = new ToolBar();
  		MenuButton dropDownMenu = new MenuButton("Servidor"); // criar dropdown


  		MenuItem connect = new MenuItem("Conectar"); // criar botão de conectar
        MenuItem disconnect = new MenuItem("Desconectar"); // criar botão de desconectar
        MenuItem startMatch = new MenuItem("Iniciar Partida"); // criar botão de desconectar

        EventHandler<ActionEvent> serverActions = serverMenuActions(); // instancia o action handler do servidor
        connect.setOnAction(serverActions);
        disconnect.setOnAction(serverActions);
        startMatch.setOnAction(serverActions);

        // EventHandler<ActionEvent> configureAction = configureServerAction();
        // configureMatch.setOnAction(configureAction);

        dropDownMenu.getItems().addAll(connect, disconnect, startMatch);
        toolbar.getItems().add(dropDownMenu);
  		border.setTop(toolbar);

  		// Tabuleiro (grid pane)
  		root = new GridPane();
  		root.setAlignment(Pos.CENTER);
  		border.setCenter(root);

  		// Painel para escolha das peças
  		FlowPane piecesFlow = new FlowPane(Orientation.VERTICAL);
     	piecesFlow.setColumnHalignment(HPos.CENTER);
     	piecesFlow.setVgap(-20);

        piecesCounterText = new Text();
        turnText = new Text();
        turnText.setText("Aguardando Partida...\n\n");
        turnText.setFill(Color.BLUE);

        piecesCounterText.setText("Peças Restantes: 20");        

     	int buttonWidth = 50;
     	int buttonHeight = 100;

     	Button road = new Button("");
     	road.getStyleClass().addAll("piece", "road");
     	road.setPrefHeight(buttonHeight);
        road.setPrefWidth(buttonWidth);

     	Button wall = new Button("");
     	wall.getStyleClass().addAll("piece", "wall");
     	wall.setPrefHeight(buttonHeight);
        wall.setPrefWidth(buttonWidth);

        king = new Button("");
        king.getStyleClass().addAll("piece", "king");
        king.setPrefHeight(buttonHeight);
        king.setPrefWidth(buttonWidth);

     	piecesFlow.getChildren().addAll(turnText, piecesCounterText, road, wall, king);


     	piecesFlow.setAlignment(Pos.CENTER);
     	border.setLeft(piecesFlow);
     	border.setMargin(piecesFlow, new Insets(0,0,0,18));

     	// Painel da pilha de peças
     	FlowPane stackFlow = new FlowPane(Orientation.VERTICAL);
     	stackFlow.setVgap(10);

     	Text stackText = new Text();
		  stackText.setText("Pilha de Peças:");

     	stackList = new ListView<String>();
		ObservableList<String> stackItems = FXCollections.observableArrayList ("");
		stackList.setItems(stackItems);

		stackFlow.getChildren().addAll(stackText, stackList);
		stackFlow.setAlignment(Pos.CENTER);
		border.setRight(stackFlow);
		border.setMargin(stackFlow, new Insets(0,20,0,0));


  		// Criação da cena do jogo, importando o CSS
  		scene = new Scene(border, 900, 500);
		scene.getStylesheets().add("game.css");
		scene.setCursor(Cursor.DEFAULT); // setar cursor para facilitar mudar nos action listeners

		// Configuração da tela
  		board.setTitle("Tak"); // título
  		board.setScene(scene); // cena
		board.setResizable(false);
		board.sizeToScene();
  		board.show();

  		// Chamada de funçao para adicionar os botões do tabuleiro
  		this.drawGrid(interfaceJogador.getTamanhoTabuleiro(), root);

  		// Funções para adicionar action listeners nos botões de peças
  		addPieceActionHandler(king, "king.png", scene);
  		addPieceActionHandler(road, "road.png", scene);
  		addPieceActionHandler(wall, "wall.png", scene);
  			
 	}

 	private void addPieceActionHandler(Button piece, String image, Scene scene) {
		EventHandler<MouseEvent> eventHandler = new EventHandler<MouseEvent>() { 
   			@Override 
   			public void handle(MouseEvent e) {
   				if(scene.getCursor() == Cursor.DEFAULT) {
   					Image cursor = new Image(image); 
      				scene.setCursor(new ImageCursor(cursor));
      				atualizarTipoJogada(image);
      				imagemJogada = image;
      			}
      			else {
      				scene.setCursor(Cursor.DEFAULT);
      				tipoJogada = -1;
      				imagemJogada = "";
      			}
      		}
		};
		piece.addEventFilter(MouseEvent.MOUSE_CLICKED, eventHandler);
 	}

 	private void atualizarTipoJogada(String imagem) {
 		switch(imagem) {
 			case "road.png": tipoJogada = 0;
 				break;

 			case "wall.png": tipoJogada = 1;
 				break;

 			case "king.png": tipoJogada = 2;
 				break;

 			default: tipoJogada = 3;
 				break;
 		}
 	}

 	// Método que cria o grid de botões
 	private void drawGrid(int nRowsAndColumns, GridPane root) {
        // Classes que vão ser importadas do css
 		String color1 = "game-button1";
 		String color2 = "game-button2";

        int side = 50;

        for(int i = 0; i < nRowsAndColumns; i++) {
            for(int j = 0; j < nRowsAndColumns; j++) {
                Button button = new Button();
                button.setPrefHeight(side);
                button.setPrefWidth(side);
                button.setMinSize(side, side);
    			button.setMaxSize(side, side);
                button.setAlignment(Pos.CENTER);
                button.setText("");
                // Se a linha e a coluna forem par OU a linha
                // e a coluna forem ímpares, pinta da cor1
                if (((i % 2 == 0) && (j % 2 == 0)) || ((i % 2 == 1) && (j % 2 == 1)))
                    button.getStyleClass().addAll(color1);
                else
                    button.getStyleClass().addAll(color2);
                root.setRowIndex(button, i);
                root.setColumnIndex(button, j);

                // Adicionar action listner no botão
                addBoardButtonsActionHandlers(button);          


                // Adiciona ao painel
                root.getChildren().add(button);
            }
        }
 	}

 	// Action handlers dos botões do tabuleiro
 	private void addBoardButtonsActionHandlers(Button button) {
 		// Handler para quando o mouse entra
		EventHandler<MouseEvent> eventHandlerOn = new EventHandler<MouseEvent>() { 
   			@Override 
   			public void handle(MouseEvent e) {
   				int a = root.getRowIndex(button);
				int b = root.getColumnIndex(button);
   				stackList.setItems(FXCollections.observableArrayList(interfaceJogador.getListaPecasCasa(a, b)));
   				button.getStyleClass().add("over");
      		}
		};
		// Handler para quando o mouse sai
		EventHandler<MouseEvent> eventHandlerOff = new EventHandler<MouseEvent>() { 
   			@Override 
   			public void handle(MouseEvent e) {
   				ObservableList<String> emptyList = FXCollections.observableArrayList ("");
   				stackList.setItems(emptyList);
   				button.getStyleClass().remove("over");
      		}
		};

		EventHandler<MouseEvent> eventHandlerClick = new EventHandler<MouseEvent>() { 
   			@Override 
   			public void handle(MouseEvent e) {
   				try {
   					int a = root.getRowIndex(button);
					int b = root.getColumnIndex(button);
   					if(tipoJogada == -1 && interfaceJogador.meuTurno()) {
   						int peca = interfaceJogador.getTopoPilhaCasa(a, b);
   						if(peca != -1) {
   							if(interfaceJogador.verificarCor(peca)) {
		   						TextInputDialog textInputDialog = new TextInputDialog(""); 
						        textInputDialog.setTitle("Mover peças");
						        textInputDialog.setHeaderText("Compor pilha de movimentação");
						        textInputDialog.setContentText("Escolha o tamanho da Pilha. O máximo são 5 peças.");
						        textInputDialog.showAndWait();

						        try {
						        	contadorPilhaMovimentacao = Integer.parseInt(textInputDialog.getEditor().getText());
						        	contadorReserva = contadorPilhaMovimentacao;
						        	if(contadorPilhaMovimentacao > 5) {
						        		alertar("Você não pode mover mais que 5 peças por vez.");
						        	}
						        	else if(contadorPilhaMovimentacao == 0) {
						        		alertar("Você precisa mover ao menos uma peça.");
						        	}
						        	else if(contadorPilhaMovimentacao < 0) {
						        		alertar("Você não pode mover um número negativo de peças.");
						        	}
						        	else if(interfaceJogador.getListaPecasCasa(a, b).size() < contadorPilhaMovimentacao) {
						        		alertar("Você tentou mover mais peças do que há na pilha escolhida!");
						        	}
						        	else {
						        		tipoJogada = 3;
						        		ultimaCasaA = a;
						        		ultimaCasaB = b;
						        		lancesMovimentacao = new ArrayList();
						        		subPilhaMovimentacao = interfaceJogador.getSubPilha(a, b, contadorPilhaMovimentacao);
						        		pecaTopoMov = (Integer) subPilhaMovimentacao.pop();
						        		interfaceJogador.removerDoTopo(a, b, contadorPilhaMovimentacao);
						        		
						        		int topo = interfaceJogador.getTopoPilhaCasa(a, b);
						        		
						        		button.getStyleClass().removeAll("king", "road", "wall", "azul", "vermelho");
						        		if(topo != -1) {
							        		int tipoJogadaL = 0;
											if(topo == 10 || topo == 20)
												tipoJogadaL = 0;
											else if (topo == 11 || topo == 21)
												tipoJogadaL = 1;
											else
												tipoJogadaL = 2;
											
							            	if(tipoJogadaL == 0)
							     				button.getStyleClass().add("road");
							     			else if(tipoJogadaL == 1)
							     				button.getStyleClass().add("wall");
							     			else if(tipoJogadaL == 2)
							     				button.getStyleClass().add("king");

							     			if(topo == 10 || topo == 11 || topo == 12)
												button.getStyleClass().add("azul");
											else if(topo == 20 || topo == 21 || topo == 22)
												button.getStyleClass().add("vermelho");
										}

						        		mudarMouse(pecaTopoMov);

						        		movA = a;
						        		movB = b;

						        	}
						    	}
						    	catch(NumberFormatException erro) {
						    		alertar("Você não inseriu um número válido!");
						    	}
					    	}
					    	else {
					    		alertar("Essa peça não é sua!");
					    	}

				    	}
   					}
   					else if(tipoJogada == 3) {
						int destino = interfaceJogador.getTopoPilhaCasa(a, b);
						if (((destino != 11 && destino != 21) || (pecaTopoMov == 12 || pecaTopoMov == 22)) 
							&& (a + 1 == ultimaCasaA || a == ultimaCasaA || a - 1 == ultimaCasaA)
							&& (b + 1 == ultimaCasaB || b == ultimaCasaB || b - 1 == ultimaCasaB)
							&& (destino != 12 && destino != 22)) {
							int tipoJogadaL = 0;
							if(pecaTopoMov == 10 || pecaTopoMov == 20)
								tipoJogadaL = 0;
							else if (pecaTopoMov == 11 || pecaTopoMov == 21)
								tipoJogadaL = 1;
							else
								tipoJogadaL = 2;
							button.getStyleClass().removeAll("king", "road", "wall", "azul", "vermelho");
			            	if(tipoJogadaL == 0)
			     				button.getStyleClass().add("road");
			     			else if(tipoJogadaL == 1)
			     				button.getStyleClass().add("wall");
			     			else if(tipoJogadaL == 2)
			     				button.getStyleClass().add("king");
			     			Lance lanceMov = new Lance(a, b, tipoJogadaL);
			     			if(pecaTopoMov == 10 || pecaTopoMov == 11 || pecaTopoMov == 12) {
								button.getStyleClass().add("azul");
								lanceMov.setCor(10);
			     			}
							else if(pecaTopoMov == 20 || pecaTopoMov == 21 || pecaTopoMov == 22) {
								button.getStyleClass().add("vermelho");
								lanceMov.setCor(20);
							}
							ultimaCasaA = a;
						    ultimaCasaB = b;
						    
						    lanceMov.setMovA(movA);
						    lanceMov.setMovB(movB);
						    lancesMovimentacao.add(lanceMov);
						    contadorPilhaMovimentacao = contadorPilhaMovimentacao - 1;
						    
						    if(contadorPilhaMovimentacao == 0) {
						    	for(int i = 0; i < lancesMovimentacao.size(); i++) {
						    		lancesMovimentacao.get(i).setTamMov(contadorReserva);
						    	}
						    	interfaceJogador.moverPecas(lancesMovimentacao);
						    	tipoJogada = -1;
						    	scene.setCursor(Cursor.DEFAULT);
						    }
						    else if(contadorPilhaMovimentacao > 0) {
						    	pecaTopoMov = (Integer) subPilhaMovimentacao.pop();
						    }

						}
						else {
							alertar("Você não pode mover essa peça para essa casa!");
						}
   						
   					}
		            else if(interfaceJogador.meuTurno()) {
		            	int verifica = interfaceJogador.getTopoPilhaCasa(a,b);
		            	if (!interfaceJogador.verificarCor(verifica)) {
		            		alertar("Você não pode posicionar uma peça nessa casa.");
		            	}
		            	else {
			            	String mensagem = interfaceJogador.podeJogar();
			            	if(mensagem.equals("")) {
			            		// IMPLEMENTAR VERIFICACAO DE COR E TIPO
			            		interfaceJogador.enviarJogada(a, b, tipoJogada);
				            	button.getStyleClass().removeAll("king", "road", "wall", "azul", "vermelho");
				            	if(tipoJogada == 0)
				     				button.getStyleClass().add("road");
				     			else if(tipoJogada == 1)
				     				button.getStyleClass().add("wall");
				     			else if(tipoJogada == 2)
				     				button.getStyleClass().add("king");

				     			int cor = interfaceJogador.getCor();
				     			if(cor == 10)
									button.getStyleClass().add("azul");
								else if(cor == 20)
									button.getStyleClass().add("vermelho");

								piecesCounterText.setText("Peças Restantes: " + interfaceJogador.getNumeroPecas());
			            	}
			            	else {
			            		alertar(mensagem);
			            	}
				            	
				            scene.setCursor(Cursor.DEFAULT);
				     		tipoJogada = -1;
				     		stackList.setItems(FXCollections.observableArrayList(interfaceJogador.getListaPecasCasa(a, b)));
			     		}
		            }
		            else {
		            	alertar("Não é sua vez de jogar!");
		            }
	            		
		        } catch (InterruptedException ex) {
		            System.out.println("Erro ao realizar jogada");
		        }
      		}
		};

		button.addEventFilter(MouseEvent.MOUSE_ENTERED, eventHandlerOn);
		button.addEventFilter(MouseEvent.MOUSE_EXITED, eventHandlerOff);
		button.addEventFilter(MouseEvent.MOUSE_CLICKED, eventHandlerClick);
 	}

 	// Action handler dos botões do menu
 	private EventHandler<ActionEvent> serverMenuActions() {
        return new EventHandler<ActionEvent>() {

            public void handle(ActionEvent event) {
                MenuItem mItem = (MenuItem) event.getSource();
                String name = mItem.getText();
                
                getServerAlert(name.toLowerCase()).showAndWait();
                
            }
        };
    }

    public void atualizarCasa(int a, int b, int tipoJogadaAdv, int cor) {
    	Button button = (Button) root.getChildren().get(a*interfaceJogador.getTamanhoTabuleiro() + b);
    	button.getStyleClass().removeAll("road", "wall", "king", "azul", "vermelho");
    	if(tipoJogadaAdv == 0)
			button.getStyleClass().add("road");
		else if(tipoJogadaAdv == 1)
			button.getStyleClass().add("wall");
		else if(tipoJogadaAdv == 2)
			button.getStyleClass().add("king");

		if(cor == 10) {
			button.getStyleClass().remove("vermelho");
			button.getStyleClass().add("azul");
		}
		else if(cor == 20){
			button.getStyleClass().remove("azul");
			button.getStyleClass().add("vermelho");			
		}
    }

    private void configurarServidor() {
    	TextInputDialog textInputDialog = new TextInputDialog("localhost"); 
        textInputDialog.setTitle("Configurar");
        textInputDialog.setHeaderText("Configuração do Servidor");
        textInputDialog.setContentText("Escolha o IP do Servidor");
        textInputDialog.showAndWait();

        // set the text of the label 
        interfaceJogador.setIP(textInputDialog.getEditor().getText());


    	textInputDialog = new TextInputDialog("jogador"); 
        textInputDialog.setTitle("Configurar");
        textInputDialog.setHeaderText("Configuração do Servidor");
        textInputDialog.setContentText("Escolha o nome do jogador");
        textInputDialog.showAndWait();

        // set the text of the label 
        interfaceJogador.setNomeJogador(textInputDialog.getEditor().getText());
    }

    // Função que cria os alerts para mostrar quando clica em um botão do menu de servidor

    private Alert getServerAlert(String serverFunction) {
        Alert alert = new Alert(AlertType.INFORMATION);
        alert.setHeaderText(null);
        String message = "";
        switch(serverFunction) {
            case "conectar":
            	configurarServidor();
                alert.setTitle("Conectar-se");
                message = interfaceJogador.conectar();
                break;
        
            case "desconectar":
                alert.setTitle("Desconectar-se");
                message = interfaceJogador.desconectar();
                break;

            case "iniciar partida":
                alert.setTitle("Iniciar Partida");
                message = interfaceJogador.comecarPartida();
                emJogo = true;
                break;

            default:
                alert.setTitle("Error");
                alert.setContentText("Erro ao tentar chamar funçao do servidor!");
        }

        alert.setContentText(message);

        return alert;
    }

    public void alertar(String mensagem) {
    	Alert alerta = new Alert(AlertType.INFORMATION);
    	alerta.setHeaderText(null);
    	alerta.setTitle("Mensagem");
    	alerta.setContentText(mensagem);
    	alerta.showAndWait();
    }

    public void hideKing() {
    	king.setVisible(false);
    }

    public void setTurnText(int type) {
    	if(type == 1) {
	    	turnText.setText("Sua vez!\n\n");
	        turnText.setFill(Color.GREEN);
    	}
    	else if(type == 2) {
	    	turnText.setText("Turno do oponente\n\n");
	        turnText.setFill(Color.RED);
    	}
    	else if(type == 3) {
	    	turnText.setText("Aguardando Partida...\n\n");
	        turnText.setFill(Color.BLUE);
    	}
    	else if(type == 4) {
    		turnText.setText("Você Venceu!\n\n");
	        turnText.setFill(Color.GREEN);
    	}
    	else if(type == 5) {
    		turnText.setText("Você Perdeu!\n\n");
	        turnText.setFill(Color.RED);
    	}
    	else {
    		alertar("Erro ao trocar mensagem de passagem de turno!");
    	}
    }

    private void mudarMouse(int pecaTopoMov) {
    	String image = "";
    	
		if(pecaTopoMov == 10 || pecaTopoMov == 20)
			image = "road.png";
		else if (pecaTopoMov == 11 || pecaTopoMov == 21)
			image = "wall.png";
		else
			image = "king.png";
    	Image cursor = new Image(image); 
		scene.setCursor(new ImageCursor(cursor));
		imagemJogada = image;
    }

    public void limparTabuleiro() {
    	Button button;
    	int t = interfaceJogador.getTamanhoTabuleiro();
    	for(int i = 0; i < t; i++) {
    		for(int j = 0; j < t; j++) {
    			button = (Button) root.getChildren().get(i*t + j);
    			button.getStyleClass().removeAll("king", "road", "wall", "azul", "vermelho");    		
    		}
    	}
    }

    public void limparCasa(int a, int b) {
    	Button button = (Button) root.getChildren().get(a*interfaceJogador.getTamanhoTabuleiro() + b);
    	button.getStyleClass().removeAll("king", "road", "wall", "azul", "vermelho");
    }

    public void atualizarNumeroPecas() {
    	piecesCounterText.setText("Peças Restantes: " + interfaceJogador.getNumeroPecas());
    }

}
