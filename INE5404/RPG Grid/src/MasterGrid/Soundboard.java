package MasterGrid;

import java.io.*;
import java.net.URL;
import javax.sound.sampled.*;
import javax.swing.JOptionPane;


public class Soundboard {
 public void playSound(String soundName)
         
 {
   try 
   {
   AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(new File(soundName).getAbsoluteFile( ));
   Clip clip = AudioSystem.getClip( );
   clip.open(audioInputStream);
   clip.start( );
   }
   catch(UnsupportedAudioFileException e){
       JOptionPane.showMessageDialog(null, "Formato invalido, escolha um arquivo .wav");
   }
   catch(Exception ex)
   {
     System.out.println("Error with playing sound.");
     ex.printStackTrace( );
   }
 }

}