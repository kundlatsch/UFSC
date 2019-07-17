package MasterGrid;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.IOException;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

//@SuppressWarnings("serial")
public class LibraryPane extends JPanel {
	private String libraryContent;
	private JLabel[] titleArray = new JLabel[10];
	private JComboBox<String> comboBox;
	private int ntCounter = 1;

	public LibraryPane() {
		GenericFileReader libraryIn = new GenericFileReader("library.txt");
		try {
			libraryContent = libraryIn.readFile();
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "Erro ao acessar biblioteca");
		}

		Library library = new Library();

		String[] content = libraryContent.split(";");
		for (int i = 0; i < content.length - 1; i += 2) {
			library.addText(content[i], content[i + 1]);
		}
		JLabel choose = new JLabel("Topico: ");
		comboBox = new JComboBox<String>(library.getTitles());
		JTextField title = new JTextField(40);

		this.add(choose);
		this.add(comboBox);
		this.add(title);

		JTextArea textArea = new JTextArea(20, 40);
		this.add(textArea);

		JButton add = new JButton("Novo");
		JButton save = new JButton("Salvar");
		JButton remove = new JButton("Deletar");

		add(add);
		add(save);
		add(remove);

		comboBox.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				title.setText(library.getTitle(comboBox.getSelectedIndex()));
				textArea.setText(library.getBody(comboBox.getSelectedIndex()));
			}
		});

		save.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				int s = comboBox.getSelectedIndex();
				FileWriter libraryOut = new FileWriter("library.txt");
				library.editText(s, title.getText(), textArea.getText());
				libraryOut.write(library.toString());
				libraryOut.close();
				DefaultComboBoxModel model = new DefaultComboBoxModel(library.getTitles());
				comboBox.setModel(model);
				comboBox.setSelectedIndex(s);
			}
		});

		add.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				library.addText("Novo Topico " + ntCounter, "Novo Texto");
				comboBox.addItem("Novo Topico " + ntCounter);
				comboBox.setSelectedIndex(comboBox.getItemCount() - 1);
				ntCounter++;
			}

		});

		remove.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				int i = comboBox.getSelectedIndex();
				library.removeText(i);
				DefaultComboBoxModel model = new DefaultComboBoxModel(library.getTitles());
				comboBox.setModel(model);
				if (i > 0)
					comboBox.setSelectedIndex(i - 1);
				title.setText(library.getTitle(i));
				textArea.setText(library.getBody(i));
				FileWriter libraryOut = new FileWriter("library.txt");
				libraryOut.write(library.toString());
				libraryOut.close();
			}

		});

	}
}