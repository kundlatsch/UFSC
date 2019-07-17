
package MasterGrid;

import javax.swing.JFrame;

public class Table {
    public static void main(String[] args){
                DMInterface desktopFrame = new DMInterface();
		desktopFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		desktopFrame.setSize(1080, 720); // set frame size
		desktopFrame.setVisible(true); // display frame
    }
}
