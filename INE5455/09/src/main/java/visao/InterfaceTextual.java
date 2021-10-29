package visao;

import javax.swing.JOptionPane;

public class InterfaceTextual {

	public InterfaceTextual() {	}
	
	public void mostreMsg(String msg) {
		JOptionPane.showMessageDialog(null, msg);
	}
	
	public String recebaValor(String msg) {
		return JOptionPane.showInputDialog(msg);
	}
	
	public int oferecaOpcoes() {
		try {
			return Integer.parseInt(JOptionPane.showInputDialog("Informe o que voce deseja fazer:\n\n1 - para cadastrar usuario.\n"
					+ "2 - para cadastrar produto.\n\n3 - para ver os usuarios cadastrados.\n4 - para ver os produtos em leilao.\n"
					+ "5 - para ver os produtos vendidos.\n6 - para ver os produtos vencidos.\n\n7 - para dar um lance.\n"
					+ "8 - para ver os lances efetuados.\n\nQualquer outro numero para fechar."));
		}
		catch(Exception e) {
			return 0;
		}
	}
}