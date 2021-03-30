/**
 * @author Roberto Willrich
 * 
 * Aplicação Java usada para gerar arquivo CUIF a partir de um arquivo BMP (Windows Bitmap)
 * 
 * Exemplo de uso: java bmp2cuif --version 1 lena.bmp lena.cuif
 *  
 */
package br.ufsc.ine5431.praticaiv;

public class bmp2cuif {
	
	public static void main(String[] args) {
		int version = 0;
		// Analisa argumentos
	    if (args.length!=4) {
	    	System.out.println("Número errado de argumentos:" + args.length);
	    	System.out.println("Sintaxe: java bmp2cuif -v <version> <arquivo entrada> <arquivo saida>");
	    	return;
	    }
	    String arg1 = args[0].toLowerCase().trim();
	    if (!arg1.equals("-v")) {
	    	System.out.println("Argumento desconhecido: " + args[0]);
	    	System.out.println("Sintaxe: java bmp2cuif -v <version> <arquivo entrada> <arquivo saida>");
	    	return;
	    }
	    try {
	    	version = Integer.parseInt(args[1]);
	    	if ((version<1) || (version>4)) {
		    	System.out.println("Versão não suportada");
		    	return;
	    	}
	    } catch (NumberFormatException e) {
	    	System.out.println("Versão não suportada");
	    }
	    
	    // Leitura dos demais argumentos
	    String inputFile = args[2];
	    String outputFile = args[3];
	    
	    try {
	    	// Leitura de arquivo bmp 
	    	Bitmap bmpimg = new Bitmap(inputFile);
	    	//	bmpimg.printHeader();
	    	
	    	// modifique numero_estudantes e id_estudante indicando o número de membros da equipe e a matrícula dos membros
	    	int numero_de_estudantes = 3;
	    	int[] id_estudante = {17100508, 17102810, 19250729};
	    	
	    	// Geração e salvamento do arquivo CUIF
	    	Cuif cuiffile = new Cuif(bmpimg, version, numero_de_estudantes,id_estudante);
	    	cuiffile.save(outputFile);

	    } catch (Exception ioex) {
	    	ioex.printStackTrace();
	    }
	  }
  }
