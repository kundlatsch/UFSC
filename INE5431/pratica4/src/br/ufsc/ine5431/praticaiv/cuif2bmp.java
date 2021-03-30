/**
 * @author Roberto Willrich
 * 
 * Aplicação Java usada para gerar arquivo BMP (Windows Bitmap)a partir de um arquivo CYUF
 *  
 * Exemplo de uso: java cuif2bmp lena.cuif lenadecodificada.bmp
 *  
 */
package br.ufsc.ine5431.praticaiv;

public class cuif2bmp {
   
	public static void main(String[] args) {
		
		// Analisa argumentos
	    if (args.length!=2) {
	    	System.out.println("Número errado de argumentos:" + args.length);
	    	System.out.println("Sintaxe: java cuif2bmp  <arquivo CUIF entrada> <arquivo BMP saida>");
	    	return;
	    }

	    String inputFile = args[0];
	    String outputFile = args[1];
	    
	    try { 		    
	    	
	    	// Gera imagem Bitmap a partir de Cuif
	    	Bitmap bmpimg = new Bitmap();
	    	bmpimg.cuiftoBitmap(inputFile);
	    	bmpimg.save(outputFile);
	    	
	    } catch (Exception ioex) {
	    	System.out.println(ioex);
	    }
	  }
  }
