
import java.awt.image.*;
import java.net.*;
import javax.imageio.ImageIO;
import javax.swing.*;


public class ImageApp   {
	
	// Leitura da imagem
	public static BufferedImage loadImage(String surl) {  
		BufferedImage bimg = null;  
		try {  
			URL url = new URL(surl);
			bimg = ImageIO.read(url);  
			//bimg = ImageIO.read(new File("D:/Temp/mundo.jpg"));
		} catch (Exception e) {  
			e.printStackTrace();  
		}  
		return bimg;  
	}  
	
	public void apresentaImagem(JFrame frame, BufferedImage img) {
		frame.setBounds(0, 0, img.getWidth(), img.getHeight());
		JImagePanel panel = new JImagePanel(img, 0, 0);
		frame.add(panel);
		frame.setVisible(true);
	}
	
	public static BufferedImage criaImagemRGB() {
		BufferedImage img = new BufferedImage(200, 200, BufferedImage.TYPE_INT_RGB);

		WritableRaster raster = img.getRaster();
		
		for(int h=0;h<img.getHeight();h++) //Percorre a horizontal
			for(int w=0;w<img.getWidth();w++) {//Percorre a vertical
				raster.setSample(w,h,0,220); // Componente Vermelho
				raster.setSample(w,h,1,219); // Componente Verde
				raster.setSample(w,h,2,97);  // Componente Azul
			} 
		return img;
	}
	
	public static BufferedImage criaImagemCinza(BufferedImage bufferedImage) {
		int width = bufferedImage.getWidth();
		int height = bufferedImage.getHeight();
		BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);
		WritableRaster raster = img.getRaster();

		for(int h = 0; h < height; h++) {
			for(int w = 0; w < width; w++) {
				int rgb = bufferedImage.getRGB(w, h);
				int r = (int)((rgb&0x00FF0000)>>>16);
				int g = (int)((rgb&0x0000FF00)>>>8);
				int b = (int)(rgb&0x000000FF);
				double y = calcularLumiancia(r,g,b);
				raster.setSample(w, h, 0, y);
			}
		}
		return img;
	}

	private static double calcularLumiancia(int r, int g, int b) {
		return (0.3*r) + (0.59*g) + (0.11*b);
	}

	public static BufferedImage[] criaImagemSplitRGB(BufferedImage bufferedImage) {
		int width = bufferedImage.getWidth();
		int height = bufferedImage.getHeight();
		BufferedImage[] img = new BufferedImage[3];
		for(int i = 0; i < 3; i++) {
			img[i] = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
			WritableRaster raster = img[i].getRaster();

			for (int h = 0; h < height; h++) {
				for (int w = 0; w < width; w++) {
					int rgb = bufferedImage.getRGB(w, h);
					int r = (int) ((rgb & 0x00FF0000) >>> 16);
					int g = (int) ((rgb & 0x0000FF00) >>> 8);
					int b = (int) (rgb & 0x000000FF);
					double y = calcularLumiancia(r, g, b);
					raster.setSample(w, h, i, y);
				}
			}
		}
		return img;
	}

	public static BufferedImage criaImagemBinaria(BufferedImage bufferedImage) {
		int width = bufferedImage.getWidth();
		int height = bufferedImage.getHeight();
		BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_BINARY);
		WritableRaster raster = img.getRaster();
		WritableRaster rasterPB = bufferedImage.getRaster();

		for(int h = 0; h < height; h++) {
			for(int w = 0; w < width; w++) {
				int[] p = new int[1];
				rasterPB.getPixel(w, h, p);
				if(p[0] > 127) {
					raster.setSample(w, h, 0, 1);
				} else {
					raster.setSample(w, h, 0, 0);
				}
			}
		}
		return img;
	}
	
	// Imprime valores dos pixeis de imagem RGB
	public static void  imprimePixeis(BufferedImage bufferedImage) {
		for(int h=0;h<bufferedImage.getHeight();h++) //Percorre a horizontal
			for(int w=0;w<bufferedImage.getWidth();w++) {//Percorre a vertical
				int rgb = bufferedImage.getRGB(w,h);
				int r = (int)((rgb&0x00FF0000)>>>16); // componente vermelho
				int g = (int)((rgb&0x0000FF00)>>>8); // componente verde
				int b = (int)(rgb&0x000000FF); //componente azul
				System.out.print("at ("+w+","+h+"): ");
				System.out.println(r+","+g+","+b);
			}
	}

	private static int[] getMean(int pixels[]) {
		int r = 0;
		int g = 0;
		int b = 0;
		int n = 0;
		for (int i=0; i<pixels.length; i++) {
			// if (pixels[i] != null) {
			r += (int)((pixels[i]&0x00FF0000)>>>16); // componente vermelho
			g += (int)((pixels[i]&0x0000FF00)>>>8); // componente verde
			b += (int)(pixels[i]&0x000000FF); //componente azul
			n++;
			// }
		}
		r /= n;
		g /= n;
		b /= n;

		return new int[]{r,g,b};
	}

	public static BufferedImage reduzirResolucao(BufferedImage img, int proporcao) {
		int newWidth = img.getWidth() / proporcao;
		int newHeight = img.getHeight() / proporcao;

		BufferedImage smallImg = new BufferedImage(newWidth, newHeight, BufferedImage.TYPE_INT_RGB);

		WritableRaster raster = smallImg.getRaster();

		for(int h=0; h<img.getHeight(); h+=proporcao) {
			for(int w=0; w<img.getWidth(); w+=proporcao) {
				int k = 0;
				int pixels[] = new int[proporcao*proporcao];
				for(int i=h; i<h+proporcao; i++){
					for(int j=w; j<w+proporcao; j++){
						pixels[k] = img.getRGB(j,i);
						k++;
					}
				}
				int[] rgb = getMean(pixels);
				int w2 = w / proporcao;
				int h2 = h / proporcao;
				raster.setSample(w2,h2,0,rgb[0]); // Componente Vermelho
				raster.setSample(w2,h2,1,rgb[1]); // Componente Verde
				raster.setSample(w2,h2,2,rgb[2]);  // Componente Azul
			}
		}

		return smallImg;
	}

	public static void main(String[] args) {
		ImageApp ia = new ImageApp();
		// Required image: https://www.inf.ufsc.br/~roberto.willrich/INE5431/circle.png
		// Alternative image: http://www.inf.ufsc.br/~roberto.willrich/INE5431/peixe.png
		BufferedImage imgJPEG = loadImage("https://www.inf.ufsc.br/~roberto.willrich/INE5431/circle.png");

		// Questão 1
		// TODO: concluir o método utilizado abaixo
		BufferedImage imgReduzida = reduzirResolucao(imgJPEG, 4);

		// Questão 2
		BufferedImage imgCinza = criaImagemCinza(imgJPEG);
		
		// Questão 3
		BufferedImage imgBinaria = criaImagemBinaria(imgCinza);

		// Questão 4
		BufferedImage[] imgSplitRGB = criaImagemSplitRGB(imgJPEG);

		System.out.println(imgJPEG.getWidth());
		System.out.println(imgJPEG.getHeight());

		System.out.println(imgReduzida.getWidth());
		System.out.println(imgReduzida.getHeight());

		ia.apresentaImagem(new JFrame("imgJPEG"), imgJPEG);
		ia.apresentaImagem(new JFrame("imgReduzida"), imgReduzida);
		ia.apresentaImagem(new JFrame("imgCinza"), imgCinza);
		ia.apresentaImagem(new JFrame("imgBinaria"), imgBinaria);

		// Printa as 3 imagens do SplitRGB
		for(int i = 0; i < 3; i++){
			String title = "img";
			switch (i){
				case 0:
					title += "SplitRed";
					break;
				case 1:
					title += "SplitGreen";
					break;

				case 2:
					title += "SplitBlue";
					break;
			}

			ia.apresentaImagem(new JFrame(title), imgSplitRGB[i]);
		}

		// imprimePixeis(imgJPEG);
	}
}