package br.ufsc.ine5431.praticaiv;
import java.lang.*;

public final class PSNR {

	/*
	 *  Ferramenta que calcula o PSNR entre um arquivo BMP original e um arquivo BMP decodificado
	 */
	public static void main(String[] args) {
		if (args.length!=2) {
	    	System.out.println("Número errado de argumentos:" + args.length);
	    	System.out.println("Sintaxe: java PSNR  <arquivo BMP original> <arquivo BMP decodificado>");
	    	return;
	    }

	    String original = args[0];
	    String decodificado = args[1];

	    try {
		    Bitmap bmporiginal = new Bitmap(original);
		    Bitmap bmpdecodificado = new Bitmap(decodificado);
	  		System.out.println("Relação de Sinal-Ruído de Pico (PSNR): "
	  				+ psnr(bmporiginal.raster,bmpdecodificado.raster,24) + " dB");

	    } catch (Exception e) {
	    	e.getMessage();
	    	e.getStackTrace();
	    }

	}

	 private static double psnr(int[][][] original, int[][][] decodificado, int bpp) {
		 /*
	    	 * Implemente o cálculo do PSNR
	    	 */

		 double logaritmando = Math.pow((Math.pow(2,bpp) - 1) / mse(original, decodificado),2);

		 double result = 10 * Math.log10(logaritmando);

		 return result;
	  }
	 
	 private static double mse(int[][][] original, int[][][] decodificado)  {
		 /*
		  * Implemente aqui o cálculo do MSE. Dica: não esqueça de aplicar o cast (double) e divisões de números inteiros
		  */

		 int nlinhas = original[0].length;
		 int ncolunas = original.length;

		 double sum = 0.0;
		 double n = nlinhas * ncolunas * 3.0;

		 for (int i=0;i<nlinhas;i++) {  //percorre linhas
			 for (int j=0;j<ncolunas;j++) {  //percorre colunas
				 for (int p=0;p<3;p++) {  //percorre componentes R, G e B
					 System.out.println("Original Componente [" +p+ "] em [" +i+","+j+"]:"+original[i][j][p]);
					 System.out.println("Decodificado Componente ["+p+"] em ["+i+","+j+"]:"+decodificado[i][j][p]);
					 sum += Math.pow(original[i][j][p] - decodificado[i][j][p],2);
				 }
			 }
		 }


		 return sum/n;
	}

}
