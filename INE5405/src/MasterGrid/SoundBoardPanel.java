package MasterGrid;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;

//@SuppressWarnings("serial")
public class SoundBoardPanel extends JPanel {
   public SoundBoardPanel() {
				
    
                Soundboard sb = new Soundboard();
                JLabel label = new JLabel("Selecione o efeito desejado");
                JFileChooser chooser = new JFileChooser();
                
               ImageIcon sword = new ImageIcon(getClass().getResource("/MasterGrid/swordBattle.png")); 
                ImageIcon spell = new ImageIcon(getClass().getResource("/MasterGrid/spell.png")); 
                ImageIcon monster = new ImageIcon(getClass().getResource("/MasterGrid/monster.png"));
                ImageIcon lvlup = new ImageIcon(getClass().getResource("/MasterGrid/lvlup.png"));
                ImageIcon bow = new ImageIcon(getClass().getResource("/MasterGrid/bow.png")); 
                ImageIcon folder = new ImageIcon(getClass().getResource("/MasterGrid/folder.png"));
                
                JButton sound1 = new JButton(sword);
                JButton sound2 = new JButton(spell);
                JButton sound3 = new JButton(monster);
                JButton sound4 = new JButton(lvlup);
                JButton sound5 = new JButton(bow);
                JButton soundUser = new JButton(folder);
                
                sound1.setPreferredSize(new Dimension(32,32));
                sound2.setPreferredSize(new Dimension(32,32));
                sound3.setPreferredSize(new Dimension(32,32));
                sound4.setPreferredSize(new Dimension(32,32));
                sound5.setPreferredSize(new Dimension(32,32));
                soundUser.setPreferredSize(new Dimension(32,32));
                
                //BOTAO 1 - SOM DE ESPADAS COLIDINDO
                sound1.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        sb.playSound("swordClash.wav");
                    }
                });
                
                //BOTAO 2 - SOM DE MAGIA LANÇADA
                sound2.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        sb.playSound("magic.wav");
                    }
                });
                
                //BOTAO 3 - SOM DE MONSTRO GRITANDO
                sound3.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        sb.playSound("monsterRoar.wav");
                    }
                });
                
                //BOTAO 4 - SOM DE LEVEL UP
                sound4.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        sb.playSound("lvlupSound.wav");
                    }
                });
                
                //BOTAO 5 - SOM DE TIRO DE ARCO
                sound5.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                       sb.playSound("arrowNbow.wav");
                       
                    }
                });
                
                //BOTAO 6 - SOM PERSONALIZADO DO USER
                soundUser.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        int audio = chooser.showOpenDialog(null);
                            chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
                            if(JFileChooser.APPROVE_OPTION == audio){
                                    File file = chooser.getSelectedFile();
                                    sb.playSound(file.getAbsolutePath());
                            }
                    }
                });
                
		this.add(label, BorderLayout.NORTH);
                this.add(sound1,BorderLayout.SOUTH);
                this.add(sound2,BorderLayout.SOUTH);
                this.add(sound3,BorderLayout.SOUTH);
                this.add(sound4,BorderLayout.SOUTH);
                this.add(sound5,BorderLayout.SOUTH);
                this.add(soundUser,BorderLayout.SOUTH);
                
		}
}