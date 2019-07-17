package MasterGrid;

import java.awt.BorderLayout;
import javax.swing.JPanel;
import javax.swing.JTextArea;

public class NotepadPane extends JPanel {
	JTextArea text;
	public NotepadPane() {
		this.setLayout(new BorderLayout() );
		text = new JTextArea();
		text.setLineWrap(true);
		this.add(text, BorderLayout.CENTER);
	}
}
