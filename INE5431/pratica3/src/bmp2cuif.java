/**
 * @author Roberto Willrich
 * 
 * Aplicação Java usada para gerar arquivo CUIF a partir de um arquivo BMP (Windows Bitmap)
 * 
 * Exemplo de uso: java bmp2cuif --version 1 lena.bmp lena.cuif
 *  
 */

public class bmp2cuif {
	
	public static void main(String[] args) {
	
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
	    	int version = Integer.parseInt(args[1]);
	    	if (version !=1) {
		    	System.out.println("Versão não suportada");
		    	return;
	    	}
	    } catch (NumberFormatException e) {
	    	System.out.println("Versão não suportada");
	    }
	    	
	    String inputFile = args[2];
	    String outputFile = args[3];
	    
	    try {
	    	// Leitura de arquivo bmp 
	    	Bitmap bmpimg = new Bitmap(inputFile);
	    	
	    	// Criação de do arquivo Cuif1 a partir da leutura do arquivo lena.bmp
	    	// modifique numero_estudantes e id_estudante indicando o número de membros da equipe e a matrícula dos membros
	    	int numero_de_estudantes = 3;
	    	int[] id_estudante = {17100508, 17102810, 19250729}; 
	    	Cuif filecuif = new Cuif(bmpimg,1, numero_de_estudantes,id_estudante);
	    	filecuif.save(outputFile);

	    } catch (Exception ioex) {
	    	ioex.printStackTrace();
	    }
	  }
  }
