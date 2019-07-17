package MasterGrid;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class SheetsPane extends JPanel {
	JLabel html;
	JPanel buttons;
	JPanel top;
	JPanel center;
	JButton add;
	JButton edit;
	JButton remove;
	String monsters;
	String characters;
	String npcs;
	JComboBox comboBox;

	JFrame editFrame;
	JTextArea editorArea;
	JScrollPane editor;
	JPanel editorButtons;
	JButton cancel;
	JButton confirm;
	JTextField editTitle;

	int cCounter = 1;
	int mCounter = 1;
	int nCounter = 1;

	public SheetsPane() {

		this.setLayout(new BorderLayout());

		GenericFileReader monsterSheets = new GenericFileReader("monsterSheets.txt");
		GenericFileReader characterSheets = new GenericFileReader("characterSheets.txt");
		GenericFileReader NPCSheets = new GenericFileReader("NPCSheets.txt");

		add = new JButton("Adicionar");
		edit = new JButton("Editar");
		remove = new JButton("Remover");

		html = new JLabel("<html><table> <tr> <th></th>Escolha uma ficha acima<th></th></tr></table></html>");

		center = new JPanel();
		center.add(html);
		this.add(center, BorderLayout.CENTER);

		editTitle = new JTextField();
		editorArea = new JTextArea();
		editor = new JScrollPane(editorArea);
		
		editorArea.setLineWrap(true);
		cancel = new JButton("Cancelar");
		confirm = new JButton("Confirmar");
		editorButtons = new JPanel();
		editFrame = new JFrame();
		editFrame.add(editTitle, BorderLayout.NORTH);
		editFrame.add(editor, BorderLayout.CENTER);
		editorButtons.add(cancel);
		editorButtons.add(confirm);
		editFrame.add(editorButtons, BorderLayout.SOUTH);

		buttons = new JPanel();

		buttons.add(add);
		buttons.add(edit);
		buttons.add(remove);
		this.add(buttons, BorderLayout.SOUTH);

		try {
			monsters = monsterSheets.readFile();
			characters = characterSheets.readFile();
			npcs = NPCSheets.readFile();

		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "Erro ao acessar fichas");
		}

		CharacterSheetManager charactersManager = new CharacterSheetManager();
		charactersManager.processSheets(characters);
		MonsterSheetManager monstersManager = new MonsterSheetManager();
		monstersManager.processSheets(monsters);
		NPCSheetManager NPCManager = new NPCSheetManager();
		NPCManager.processSheets(npcs);

		comboBox = new JComboBox<String>(cbModel(charactersManager, monstersManager, NPCManager));
		top = new JPanel();
		top.add(comboBox);
		add(top, BorderLayout.NORTH);

		comboBox.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				String text = getTrueContent(charactersManager, monstersManager, NPCManager);
				text = text.replace("\n", "").replace("\r", "");
				html.setText(text);
			}
		});

		edit.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {

				editFrame.setVisible(true);
				editFrame.setSize(400, 400); // set frame size
				editFrame.setLocation(200, 200);
				editTitle.setText(getTrueTitle(charactersManager, monstersManager, NPCManager));
				editorArea.setText(getTrueContent(charactersManager, monstersManager, NPCManager));
			}

		});

		add.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				int newIndex;
				String[] options = { "Personagem", "Monstro", "NPC" };
				int x = JOptionPane.showOptionDialog(null, "Escolha o tipo de ficha para criar", "Criar Ficha",
						JOptionPane.DEFAULT_OPTION, JOptionPane.INFORMATION_MESSAGE, null, options, options[0]);
				if (x == 0) {
					String text = charactersManager.getDefaultSheet();
					charactersManager.addSheet("Novo Personagem " + cCounter, text);// EDITAR
					cCounter++;
					newIndex = charactersManager.getTitles().length - 1;
				}

				else if (x == 1) {
					String text = monstersManager.getDefaultSheet();
					monstersManager.addSheet("Novo Monstro " + mCounter, text);// EDITAR
					mCounter++;
					newIndex = monstersManager.getTitles().length + charactersManager.getTitles().length - 1;
				}

				else {
					String text = NPCManager.getDefaultSheet();
					NPCManager.addSheet("Novo NPC " + nCounter, text);// EDITAR
					nCounter++;
					newIndex = NPCManager.getTitles().length + charactersManager.getTitles().length
							+ monstersManager.getTitles().length - 1;
				}
				DefaultComboBoxModel model = new DefaultComboBoxModel(
						cbModel(charactersManager, monstersManager, NPCManager));
				comboBox.setModel(model);
				comboBox.setSelectedIndex(newIndex);
			}

		});

		remove.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				int index = comboBox.getSelectedIndex();
				String[] options = { "Personagem", "Monstro", "NPC" };
				int x = getSheetType(comboBox.getSelectedIndex(), charactersManager, monstersManager, NPCManager);;
				if (x == 1) {
					charactersManager.removeSheet(comboBox.getSelectedIndex());
					FileWriter out = new FileWriter("characterSheets.txt");
					out.write(charactersManager.toString());
					out.close();
				}

				else if (x == 2) {
					monstersManager.removeSheet(comboBox.getSelectedIndex() - charactersManager.getTitles().length);
					FileWriter out = new FileWriter("monsterSheets.txt");
					out.write(monstersManager.toString());
					out.close();
				}

				else {
					NPCManager.removeSheet(comboBox.getSelectedIndex() - charactersManager.getTitles().length - monstersManager.getTitles().length);
					FileWriter out = new FileWriter("NPCSheets.txt");
					out.write(NPCManager.toString());
					out.close();
				}
				DefaultComboBoxModel model = new DefaultComboBoxModel(
						cbModel(charactersManager, monstersManager, NPCManager));
				comboBox.setModel(model);
			
				
				
				comboBox.setModel(model);
				if (index > 0)
					comboBox.setSelectedIndex(index - 1);
				
				
			}

		});

		cancel.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				editFrame.setVisible(false);

			}

		});

		confirm.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				editText(charactersManager, monstersManager, NPCManager);
				int s = comboBox.getSelectedIndex();
				DefaultComboBoxModel model = new DefaultComboBoxModel(
						cbModel(charactersManager, monstersManager, NPCManager));
				comboBox.setModel(model);
				comboBox.setSelectedIndex(s);
				editFrame.setVisible(false);
			}

		});
	}

	private void editText(Sheet charactersManager, Sheet monstersManager, Sheet NPCManager) {

		int s = getSheetType(comboBox.getSelectedIndex(), charactersManager, monstersManager, NPCManager);
		if (s == 1) {
			FileWriter sheetOut = new FileWriter("characterSheets.txt");
			charactersManager.editSheet(comboBox.getSelectedIndex(), editTitle.getText(), editorArea.getText());
			sheetOut.write(charactersManager.toString());
			sheetOut.close();
			// charactersManager.editSheet(comboBox.getSelectedIndex(), ,
			// content);
		} else if (s == 2) {
			FileWriter sheetOut = new FileWriter("monsterSheets.txt");
			monstersManager.editSheet(comboBox.getSelectedIndex() - charactersManager.getTitles().length,
					editTitle.getText(), editorArea.getText());
			sheetOut.write(monstersManager.toString());
			sheetOut.close();
		} else {
			FileWriter sheetOut = new FileWriter("NPCSheets.txt");
			NPCManager.editSheet(comboBox.getSelectedIndex() - charactersManager.getTitles().length
					- monstersManager.getTitles().length, editTitle.getText(), editorArea.getText());
			sheetOut.write(NPCManager.toString());
			sheetOut.close();
		}
	}

	private String[] cbModel(Sheet charactersManager, Sheet monstersManager, Sheet NPCManager) {
		String[] charactersArray = charactersManager.getTitles();
		String[] monstersArray = monstersManager.getTitles();
		String[] NPCArray = NPCManager.getTitles();

		String[] sheetsArray = new String[charactersArray.length + monstersArray.length + NPCArray.length];
		System.arraycopy(charactersArray, 0, sheetsArray, 0, charactersArray.length);
		System.arraycopy(monstersArray, 0, sheetsArray, charactersArray.length, monstersArray.length);
		System.arraycopy(NPCArray, 0, sheetsArray, charactersArray.length + monstersArray.length, NPCArray.length);
		return sheetsArray;
	}

	private int getSheetType(int index, Sheet charactersManager, Sheet monstersManager, Sheet NPCManager) {
		if (index < charactersManager.getTitles().length)
			return 1;
		else if (index < charactersManager.getTitles().length + monstersManager.getTitles().length)
			return 2;
		else
			return 3;
	}

	private String getTrueTitle(Sheet charactersManager, Sheet monstersManager, Sheet NPCManager) {
		int type = getSheetType(comboBox.getSelectedIndex(), charactersManager, monstersManager, NPCManager);
		if (type == 1) {
			return charactersManager.getTitle(comboBox.getSelectedIndex());
		} else if (type == 2) {
			return monstersManager.getTitle(comboBox.getSelectedIndex() - charactersManager.getTitles().length);
		} else {
			return NPCManager.getTitle(comboBox.getSelectedIndex() - charactersManager.getTitles().length
					- monstersManager.getTitles().length);
		}
	}

	private String getTrueContent(Sheet charactersManager, Sheet monstersManager, Sheet NPCManager) {
		int type = getSheetType(comboBox.getSelectedIndex(), charactersManager, monstersManager, NPCManager);
		if (type == 1) {
			return charactersManager.getContent(comboBox.getSelectedIndex());
		} else if (type == 2) {
			return monstersManager.getContent(comboBox.getSelectedIndex() - charactersManager.getTitles().length);
		} else {
			return NPCManager.getContent(comboBox.getSelectedIndex() - charactersManager.getTitles().length
					- monstersManager.getTitles().length);
		}
	}

}