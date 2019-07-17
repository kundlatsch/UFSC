import javax.swing.JOptionPane;
public class Interface {		
		public int solicitaInt(String msg){
			String texto = JOptionPane.showInputDialog(msg);
			return Integer.parseInt(texto);
		}

		public double solicitaDouble(String msg){
			String texto = JOptionPane.showInputDialog(msg);
			return Double.parseDouble(texto);
		}

		public String solicitaString(String msg){
			return JOptionPane.showInputDialog(msg);
		}

		public void mostrarMensagem(String msg){
			JOptionPane.showMessageDialog(null, msg);
		}
	}
