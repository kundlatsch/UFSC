
package MasterGrid;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerModel;
import javax.swing.SpinnerNumberModel;


public class DiceRollPanel extends JPanel {

	private JComboBox<String> comboBox;
	String[] dices = { "d4", "d6", "d8", "d10", "d12", "d20", "d100" };
	private JButton button;
	private JLabel label;

	public DiceRollPanel() {
		SpinnerModel value = new SpinnerNumberModel(1, // initial value
				1, // minimum value
				99, // maximum value
				1); // step
		JSpinner spinner = new JSpinner(value);
                ImageIcon rollIcon = new ImageIcon(getClass().getResource("/MasterGrid/diceIF.png"));
                comboBox = new JComboBox<String>(dices);
		comboBox.setMaximumRowCount(7);
		button = new JButton("Rolar",rollIcon);
		label = new JLabel();

		button.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				switch (comboBox.getSelectedIndex()) {
				case 0:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 4));
					break;
				case 1:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 6));
					break;
				case 2:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 8));
					break;
				case 3:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 10));
					break;
				case 4:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 12));
					break;
				case 5:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 20));
					break;
				case 6:
					label.setText("Resultado: " + DiceRoll.roll((int) spinner.getValue(), 100));
					break;
				}

			}

		}

		);

		this.add(comboBox);
		this.add(spinner, BorderLayout.NORTH);
		this.add(button);
		this.add(label);

	} // end MyJPanel constructor
}

