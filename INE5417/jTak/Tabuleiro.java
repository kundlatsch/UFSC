import br.ufsc.inf.leobr.cliente.Jogada;
import java.util.Stack;
import java.util.ArrayList;
import java.util.Collections;

public class Tabuleiro implements Jogada {
	
	protected Stack<Integer> matrizCasas[][];
	protected int tamanho;

	public Tabuleiro(int tamanho) {
		matrizCasas = new Stack[tamanho][tamanho];
		for(int i = 0; i < tamanho; i++) {
			for(int j = 0; j < tamanho; j++) {
				matrizCasas[i][j] = new Stack();
			}
		}
		this.tamanho = tamanho;
	}

	public void inserirPeca(int a, int b, int peca, int corAdversario) {
		matrizCasas[a][b].push(peca + corAdversario);
	}

	public Stack getPilha(int a, int b) {
		return matrizCasas[a][b];
	}

	public boolean verificaVitoria(int cor) {
		ArrayList<Integer>[][] matrizTemp = new ArrayList[tamanho][tamanho];

		//Stack<Integer> matrizTemp[][] = (Stack<Integer>[][]) matrizCasas.clone();
		int matriz[][] = new int[tamanho][tamanho];
		for(int i = 0; i < this.tamanho; i++) {
			for(int j = 0; j < this.tamanho; j++) {
				matrizTemp[i][j] = new ArrayList(matrizCasas[i][j]);
				Collections.reverse(matrizTemp[i][j]);
				if(matrizTemp[i][j].size() == 0)
					matriz[i][j] = -1;
				else
					matriz[i][j] = (Integer) matrizTemp[i][j].get(0);

			}
		}

		boolean retorno = false;
		// Print matriz
		// for (int linha = 0; linha < tamanho; linha++) {
  //           for (int coluna = 0; coluna < tamanho; coluna++) {
  //               System.out.print(matriz[linha][coluna]);
  //           }
  //           System.out.println("");
  //       }

		for (int linha = 0; linha < this.tamanho; linha++) {
            for (int coluna = 0; coluna < this.tamanho; coluna++) {
                if(matriz[linha][coluna] != -1) {
                    int matrizA[][] = new int[this.tamanho][this.tamanho];
                    for(int i = 0; i < this.tamanho; i++) {
                        for(int j = 0; i < this.tamanho; i++) {
                            matrizA[i][j] = 0;
                        }
                    }
                    if(verifica(matriz, linha, coluna, matrizA, 0, 0, cor, this.tamanho)) {
                        retorno = true;
                    }
                }
            }
        }

      	return retorno;
	}

	private boolean verifica(int[][] matriz, int linha, int coluna, int[][] matrizA, int ladoED, int ladoCB, int cor, int tamanho) {
        // Verificar se um lado foi encontrado

        if(linha == 0) {
            if(ladoCB == 2)
                return true;
            else
                ladoCB = 1; 
        }

        if(linha == tamanho - 1) {
            if(ladoCB == 1)
                return true;
            else
                ladoCB = 2; 
        }

        if(coluna == 0) {
            if(ladoED == 2)
                return true;
            else
                ladoED = 1; 
        }

        if(coluna == tamanho - 1) {
            if(ladoED == 1)
                return true;
            else
                ladoED = 2; 
        }

        // Observar adjacencias
        if(linha != tamanho && linha != -1 && coluna != tamanho && coluna != -1)
            matrizA[linha][coluna] = 1;

        int i = linha + 1;
        int j = coluna + 1;
        int k = linha - 1;
        int l = coluna - 1;

        try {
            if(matriz[linha][coluna + 1] != -1 && matrizA[linha][coluna + 1] == 0 && verificaCor(matriz[linha][coluna + 1], cor)) {
                if(verifica(matriz, linha, coluna + 1, matrizA, ladoED, ladoCB, cor, tamanho)){
                    return true;
                }
            }
        } catch (IndexOutOfBoundsException e) {}

        try {
            if(matriz[linha + 1][coluna] != -1 && matrizA[linha + 1][coluna] == 0 && verificaCor(matriz[linha + 1][coluna], cor)) {
                if(verifica(matriz, linha + 1, coluna, matrizA, ladoED, ladoCB, cor, tamanho))
                    return true;
            }
        } catch (IndexOutOfBoundsException e) {}

        try {
            if(matriz[linha][coluna - 1] != -1 && matrizA[linha][coluna - 1] == 0 && verificaCor(matriz[linha][coluna - 1], cor)) {
                 if(verifica(matriz, linha, coluna + 1, matrizA, ladoED, ladoCB, cor, tamanho))
                    return true;
            }
        } catch (IndexOutOfBoundsException e) {}

        try {
            if(matriz[linha - 1][coluna] != -1 && matrizA[linha - 1][coluna] == 0 && verificaCor(matriz[linha - 1][coluna], cor)) {
                if(verifica(matriz, linha - 1, coluna, matrizA, ladoED, ladoCB, cor, tamanho))
                    return true;
            }
        } catch (IndexOutOfBoundsException e) {}

        return false;
    }

    private boolean verificaCor(int peca, int cor) {
    	if(peca == 11 || peca == 21) {
    		return false;
    	}
    	else {
    		if(cor == 10) {
	    		if(peca < 19)
	    			return true;
	    		else
	    			return false;
	    	}
	    	else if(cor == 20) {
	    		if(peca > 19)
	    			return true;
	    		else
	    			return false;
	    	}
    	}
    	return false;
    }

}