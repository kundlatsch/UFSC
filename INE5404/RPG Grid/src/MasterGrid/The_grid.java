package MasterGrid;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;

public class The_grid extends JFrame {

	private static final long serialVersionUID = 1L;
	boolean trade=false;
	private Icon tradeIcon=null;
	private JPopupMenu popupMenu;
	JMenuItem items[] = new JMenuItem[2];
	
	public The_grid(){
		JFrame frame = new JFrame();
		    

		popupMenu= new JPopupMenu();
		items[0] = new JMenuItem("Inserir icone");
		ActionListener al = new PopupActionListener();
		items[0].addActionListener(al);
		popupMenu.add(items[0]);
		items[1] = new JMenuItem("Remover icone");
		items[1].addActionListener(al);
		popupMenu.add(items[1]);
		
	    JPanel buttonPanel = new JPanel();
	    
	    buttonPanel.setLayout(new GridLayout(18,32));//1920/30=64; 1080/30=36; NEW 1920->18; 1080->32
	    for (int r = 0; r < 18; r++) {
	        for (int c = 0; c < 32; c++) {
	        	JButton button = new JButton();
	        	button.setComponentPopupMenu(popupMenu);
	        	button.setOpaque(false);
	        	button.setContentAreaFilled(false);
	        	button.addActionListener(new ActionListener() {
	                public void actionPerformed(ActionEvent e) {
	                    if (trade){
	                    	JButton button =(JButton)e.getSource();
	                    	button.setIcon(tradeIcon);
	                    	trade=false;
	                    }else{
	                    	JButton button =(JButton)e.getSource();
	                    	if(button.getIcon()!=null){
		                    	tradeIcon=button.getIcon();
		                    	button.setIcon(null);
		                    	trade=true;
	                    	}
	                    }
	                }
	            });
	            buttonPanel.add(button);
	        }
	    }
	    
	    //buttonPanel.setPreferredSize(new Dimension((int)screenSize.getWidth(), (int)screenSize.getHeight()));
	    buttonPanel.setPreferredSize(new Dimension(900,500));

	    frame.getContentPane().add(buttonPanel);
	    frame.pack();
	    frame.setVisible(true);  
    }

	 
	class PopupActionListener implements ActionListener{
		public void actionPerformed(ActionEvent e){
			if(e.getSource()==items[0]){
				JFileChooser chooser = new JFileChooser();
				int returnImg = chooser.showOpenDialog(null);
				chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
				if(returnImg == JFileChooser.APPROVE_OPTION){
					File file = chooser.getSelectedFile();
					int i = file.getAbsolutePath().lastIndexOf('.');
					if (i > 0) {
						System.out.println(file.getPath().substring(i+1));
					    if(file.getPath().substring(i+1).equals("jpg") || file.getPath().substring(i+1).equals("png")){
					    	JButton but= (JButton)popupMenu.getInvoker();
							ImageIcon icon = new ImageIcon(file.getAbsolutePath());
							but.setIcon(icon);
					    }else
					    	JOptionPane.showMessageDialog(null, "Formato invalido");
					}
				}
			}else if (e.getSource()==items[1]){
				JButton but= (JButton)popupMenu.getInvoker();
				but.setIcon(null);
			}
		}
	}
	
}