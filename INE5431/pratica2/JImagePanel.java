import java.awt.Graphics;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;

public class JImagePanel extends JPanel{
	private BufferedImage image;
	int x, y;
	public JImagePanel(BufferedImage image, int x, int y) {
		super();
		this.image = image;
		this.x = x;
		this.y = y;
	}
	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(image, x, y, null);
	}
}