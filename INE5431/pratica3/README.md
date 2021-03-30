Prática III
-----------

**Questão 1. Corrigir é o erro no código para que a imagem decodificada seja igual a imagem original. O código corrigido deve ser entregue. Dica:a implementação de tal conversão está no arquivo Bitmap.java, método cuif1toRaster (linha 219). Também pode analisar o conteúdo do arquivo original BMP e aquele decodificado, por exemplo usando <https://hexed.it/>**

**Questão 2. Há perdas nos dados da imagem na conversão bmp→cuif(CUI.1)→bmp? Expliquem.**

Não há perdas, visto que não houve compressão, sendo que a mesma informação é representada em formato diferente.

Por vias de teste foi comprovada a igualdade entre o arquivo da imagem original e o arquivo decodificado a partir do arquivo cuif.
Inclusive o hash dos dois arquivos foi igual.

**Questão 3.
Indique a vantagem de organizar os pixels nesta sequência (primeiro os valores de R, depois de G e finalmente de B) para a compressão baseada em entropia, por exemplo, DPCM? Dica: relembre primeiro oprincípio do DPCM.**

Pois tendo os canais em sequência facilita o DPCM que teria o primeiro valor de um canal do píxel e os valores dos próximos são calculados com base em uma função de predição.
Para esta função, é mais eficiente comparar separadamente cada um dos canais do que comparando a composição dos canais, pois isto apresenta menor entropia.

**Questão 4.
O que pode ser feito para gerar uma imagemCUI.1que preserve apenas um dos canais de cor (R,G ou B) do bitmap? Indique o método da classe Cuif a modificar, e entregue o método alterado para manter apenas o canal R.**

Nas linhas 72 e 79 do arquivo `Cuif.java` inserimos o valor `0` nas variáveis referentes aos canais `b` e `g`.

```diff
private void readRGB(int[][][] rasterbmp, int offset) {
    	int r, g, b; 
    	raster=new int[height][width][3];
   	 	for (int i=0; i<height; i++){
           for (int j=0; j<width; j++){
           		r = rasterbmp[i][j][0];
           		cuiffile[offset++] = (byte)(r&0xff);
           		raster[i][j][0] = r;
           }
   	 	}
   	 	for (int i=0; i<height; i++){
            for (int j=0; j<width; j++){
-            		g = rasterbmp[i][j][1];            	
+                      g = 0;
               		cuiffile[offset++] = (byte)(g&0xff);
               		raster[i][j][1] = g;
            }
    	 }
   	 	for (int i=0; i<height; i++){
            for (int j=0; j<width; j++){
-            		b = rasterbmp[i][j][2];            	
+                      b = 0;
               		cuiffile[offset++] = (byte)(b&0xff);
               		raster[i][j][2] = b;
            }
    	 }
    }

```