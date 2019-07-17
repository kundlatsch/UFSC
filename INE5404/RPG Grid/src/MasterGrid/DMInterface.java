package MasterGrid;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.util.Random;
import javax.swing.JFrame;
import javax.swing.JDesktopPane;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JInternalFrame;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerModel;
import javax.swing.SpinnerNumberModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.Box.Filler;
import javax.swing.DefaultComboBoxModel;
import javax.swing.Icon;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JColorChooser;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class DMInterface extends JFrame {

	private JDesktopPane theDesktop;
              
        
	public DMInterface() {
		super("Master Table");
		JMenuBar bar = new JMenuBar(); // create menu bar
		JMenu addMenu = new JMenu("Janelas"); // create Add menu
                JMenu options = new JMenu("Opcoes");
		addMenu.setMnemonic(KeyEvent.VK_J);
                options.setMnemonic(KeyEvent.VK_O);
		
                
                JMenuItem grid = new JMenuItem("Grid");
    		JMenuItem rollDice = new JMenuItem("Rolagem de Dados");
		JMenuItem soundBoard = new JMenuItem("Efeitos Sonoros");
		JMenuItem library = new JMenuItem("Biblioteca");
		JMenuItem characterSheets = new JMenuItem("Fichas de Personagem");
		JMenuItem notepad = new JMenuItem("Bloco de Notas");
                
                JMenuItem trocarFundo = new JMenuItem("Trocar Fundo");
                JMenuItem sobre = new JMenuItem("Sobre");
                JMenuItem sair = new JMenuItem("Sair");
                
                grid.setMnemonic(KeyEvent.VK_G);
                rollDice.setMnemonic(KeyEvent.VK_R);
                soundBoard.setMnemonic(KeyEvent.VK_S);
                library.setMnemonic(KeyEvent.VK_L);
                characterSheets.setMnemonic(KeyEvent.VK_C);
                notepad.setMnemonic(KeyEvent.VK_N);
                
                sair.setMnemonic(KeyEvent.VK_Q);
                sobre.setMnemonic(KeyEvent.VK_S);
                trocarFundo.setMnemonic(KeyEvent.VK_F);
                
                addMenu.add(grid);
		addMenu.add(rollDice); // add new frame item to Add menu
		addMenu.add(soundBoard);
                addMenu.add(library);
                addMenu.add(characterSheets);
                addMenu.add(notepad);
                
                options.add(trocarFundo);
                options.add(sobre);
                options.add(sair);
                
                bar.add(options);
		bar.add(addMenu); // add Add menu to menu bar
		setJMenuBar(bar); // set menu bar for this application

		theDesktop = new JDesktopPane(); // create desktop pane
		add(theDesktop); // add desktop pane to frame
                
                sair.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent ae) {
                      System.exit(0);
                    }
                });
                
                
                //GRID LISTENER
                grid.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        new The_grid();
                    }
                });
		//rollDice LISTENER
		rollDice.addActionListener(new ActionListener() // anonymous inner class
		{
			// display new internal window
                    @Override
			public void actionPerformed(ActionEvent event) {
				// create internal frame JInternalFrame(String title,boolean
				// resizable,boolean closable,
				// boolean maximizable,boolean iconifiable)
				JInternalFrame frame = new JInternalFrame("Dados", false, true, false, true);
				DiceRollPanel panel = new DiceRollPanel(); // create new panel
                                ImageIcon diceIcon = new ImageIcon(getClass().getResource("/MasterGrid/diceIcon.png")); 
                                frame.setFrameIcon(diceIcon);
                                frame.add(panel, BorderLayout.CENTER); // add panel
				frame.setSize(250, 110);
                                theDesktop.add(frame); // attach internal frame
				frame.setVisible(true);
                                
			}
		});
                
		//SOUNDBOARD LISTENER
		soundBoard.addActionListener(new ActionListener() // anonymous inner class
				{
					// display new internal window
					public void actionPerformed(ActionEvent event) {
						// create internal frame JInternalFrame(String title,boolean
						// resizable,boolean closable,
						// boolean maximizable,boolean iconifiable)
						JInternalFrame frame = new JInternalFrame("Sons", false, true, false, true);
						SoundBoardPanel panel = new SoundBoardPanel(); // create new panel
                                                ImageIcon iconSB = new ImageIcon(getClass().getResource("/MasterGrid/speaker.png")); 
                                                frame.setFrameIcon(iconSB);
						frame.add(panel, BorderLayout.CENTER); // add panel
						frame.setSize(200, 135);
						theDesktop.add(frame); // attach internal frame
						frame.setVisible(true); // 
					}
				});
                
	//LIBRARY LISTENER
	library.addActionListener(new ActionListener() // anonymous inner class
				{
					// display new internal window
					public void actionPerformed(ActionEvent event) {
						// create internal frame JInternalFrame(String title,boolean
						// resizable,boolean closable,
						// boolean maximizable,boolean iconifiable)
						JInternalFrame frame = new JInternalFrame("Biblioteca", false, true, false, true);
						LibraryPane panel = new LibraryPane(); // create new panel
						frame.add(panel, BorderLayout.CENTER); // add panel
						frame.setSize(500, 450);
						ImageIcon Icon = new ImageIcon(getClass().getResource("/MasterGrid/libraryIcon.png")); 
                                                frame.setFrameIcon(Icon);
                                                theDesktop.add(frame); // attach internal frame
						frame.setVisible(true); // 
					}
				});
        
        //CHARACTERSHEETS LISTENER
   characterSheets.addActionListener(new ActionListener() // anonymous inner class
                                    {
                                            // display new internal window
                                            public void actionPerformed(ActionEvent event) {
                                                    // create internal frame JInternalFrame(String title,boolean
                                                    // resizable,boolean closable,
                                                    // boolean maximizable,boolean iconifiable)
                                                    JInternalFrame frame = new JInternalFrame("Fichas", true, true, true, true);
                                                    SheetsPane panel = new SheetsPane(); // create new panel
                                                    frame.add(panel, BorderLayout.CENTER); // add panel
                                                    frame.setSize(450, 500);
                                                    ImageIcon Icon = new ImageIcon(getClass().getResource("/MasterGrid/sheet.png")); 
                                                    frame.setFrameIcon(Icon);
                                                    theDesktop.add(frame); // attach internal frame
                                                    frame.setVisible(true);
                                            }
                                    });
   
   notepad.addActionListener(new ActionListener() // anonymous inner class
           {
                   // display new internal window
                   public void actionPerformed(ActionEvent event) {
                           // create internal frame JInternalFrame(String title,boolean
                           // resizable,boolean closable,
                           // boolean maximizable,boolean iconifiable)
                           JInternalFrame frame = new JInternalFrame("Anotacoes", true, true, true, true);
                           NotepadPane panel = new NotepadPane(); // create new panel
                           frame.add(panel, BorderLayout.CENTER); // add panel
                           frame.setSize(400, 400);
                           ImageIcon Icon = new ImageIcon(getClass().getResource("/MasterGrid/sheet.png")); 
                           frame.setFrameIcon(Icon);
                           theDesktop.add(frame); // attach internal frame
                           frame.setVisible(true);
                   }
           });
        trocarFundo.addActionListener(
			new ActionListener() { //classe interna an�nima
			@Override //exibe JColorChooser quando o usu�rio clica no bot�o
			public void actionPerformed(ActionEvent arg0) {
                            Color color = Color.LIGHT_GRAY;
                            color = JColorChooser.showDialog(theDesktop, "Escolhe uma cor", color); 
				//JColorChooser.showDialog(Component,T�tulo, Cor inicial) --> component � o componente que gera o dialog. 
				
				if (color == null) //configura a cor padr�o, se nenhuma cor for retornada
					color = Color.LIGHT_GRAY;
				
				theDesktop.setBackground(color); //muda a cor de fundo do painel de conte�do
			} //fim action performed
			} //fim classe interna an�nima
		); //fim chamada addActionListener
         
                sobre.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        JInternalFrame about = new JInternalFrame("Sobre",false,true,false,false);
                        JTextArea detalhes = new JTextArea("                    UNIVERSIDADE FEDERAL DE SANTA CATARINA\n"
                                                            + "\n"
                                                            + "         Este programa foi feito com o intuíto de facilitar trabalho de\n"
                                                            + "     o um Master de RPG de mesa, tendo várias ferramentas como:\n"
                                                            + "     Grid com tokens moveis, simulação de rolagem de diversos tipos de dados,\n"
                                                            + "     Mesa de efeitos sonoros, uma biblioteca contendo quaisquer textos desejados,\n"
                                                            + "     Armazenamento para fichas de personagens, NPCs e Monstros e\n"
                                                            + "     um bloco de notas para quaisquer anotações que nao precisam ser salvas.\n"
                                                            + "\n"
                                                            + "         Este programa foi criado ,com base nas aulas\n"
                                                            + "     de Programação Orientada a Objetos 2(INE5405) ministradas pela\n"
                                                            + "     professora Cecilia Estela Giuffra Palomino e pelo \n"
                                                            + "     professor Rosvelter Coelho da Costa, agora ja aposentado.\n"
                                                            + "\n"
                                                            + "     Créditos do desenvolvimento deste Projeto aos alunos:\n"
                                                            + "     Gustavo Kundlatsch, Lucas Verdade e Teo Gallarza");
                        
                        detalhes.setEditable(false);
                        about.add(detalhes);
                        about.setSize(550,300);
                        //icone
                        theDesktop.add(about);
                        about.setVisible(true);
                    }
                });
        }
}
