Prática IV - Compressão de Entropia
===================================

Para compilar o projeto

``` shell
javac src/br/ufsc/ine5431/huffmancoding/*.java src/br/ufsc/ine5431/praticaiv/*.java src/br/ufsc/ine5431/rlecoding/*.java -d class/
```

Roteiro
-------

### Parte I

O arquivo gerado inicialmente, estava com as cores trocadas e o a posição dos pixels estava invertida.

#### 4. Gerar os arquivos JazzMan2.cuif e JazzMan2.bmp

 ```
 java bmp2cuif -v 1 JazzMan.bmp JazzMan1.cuif
 ```
 
![JazzMan1.bmp](https://raw.githubusercontent.com/kundlatsch/INE5431/master/pratica4/img/JazzMan1.bmp)

### Parte II

Correção da conversão de rgb para ycbcr em ``` colorSpace.java ``` na linha 15
```java
public static double[] rgb_to_ycbcr_base_double(double r, double g, double b) {     
    double Ey, Er, Eb;

    Ey = 0.299*r + 0.587*g + 0.144*b;
    Er = 0.701*r - 0.587*g - 0.114*b;
    Eb = -.299*r - 0.587*g + 0.886*b;

    double Cr, Cb;

    Cr = Er / 1.402;
    Cb = Eb / 1.772;

    double ycbcr[] = new double[3];

    ycbcr[0] = Ey;
    ycbcr[1] = Cb;
    ycbcr[2] = Cr;

    return ycbcr;

```

#### Gerar um arquivo chamado JazzMan2.cuif

Percebemos uma leve diferença entre as cores da imagem original e da imagem gerada.

![JazzMan1.bmp](https://raw.githubusercontent.com/kundlatsch/INE5431/master/pratica4/img/JazzMan1.bmp) ![JazzMan2.bmp](https://raw.githubusercontent.com/kundlatsch/INE5431/master/pratica4/img/JazzMan2.bmp)


![JazzMan3.bmp](https://raw.githubusercontent.com/kundlatsch/INE5431/master/pratica4/img/JazzMan3.bmp) ![JazzMan4.bmp](https://raw.githubusercontent.com/kundlatsch/INE5431/master/pratica4/img/JazzMan4.bmp)

### Parte III
``` java PSNR JazzMan.bmp JazzMan1.bmp ```
infinity dB

### Parte IV

``` java PSNR JazzMan.bmp JazzMan3.bmp```

97.31590693774297 dB

``` java PSNR JazzMan2.bmp JazzMan3.bmp```

infinity dB



### Parte V

```java PSNR JazzMan.bmp JazzMan4.bmp```
152.48907843523975 dB

Questões
--------

#### Questão 1. Há alguma compressão entre CUI.1 e CUI.2? Expliquem.

Não há compressão, apenas perda de dados na conversão.

#### Questão 2. Indique o PSNR comparando a imagem original com as imagens obtidas a partir dos arquivos CUIF.1 e CUIF2. Há perdas nos dados da imagem na conversão RGB→YCbCr→RGB? Explique porque.

O PSNR é de aproximadamente 97.31591 dB. Há perda de informação nas cores, devido à conversão de RGB para YCbCr. Dessa forma o bitmap gerado a partir desse CUIF é diferente do original.

#### Questão 3. Calcule a taxa de compressão em relação ao arquivo JazzMan1.cuif (CUIF.1 sem compressão) e JazzMan3.cuif (CUIF.3 com Codificação de Huffman). Dica: A taxa de compressão da versão V em relação à versão X é tamanho CUI.X tamanho CUI.V: 1. Por exemplo, se CUI.X resulta em 100 KB e CUI.V resulta em 50 KB, a taxa decompressão de V em relação a X é de 2:1. Comente também no relatório o princípio da compressão usada na codificação de Huffman, para isto analise o histograma e a tabela de códigos impressos.

Taxa de compressão de aproximadamente 1,20755:1

A compressão usando a codificação de Huffman basicamente representa com menos bits os valores mais comuns de uma mensagem, vide a tabela em que 0000 = 106, e sendo essa a menor entrada na tabela sabemos que 106 é o valor mais comum e a entrada 11111111 = 136 sendo 136 o valor menos comum.

#### Questão 4. Indique o PSNR comparando a imagem original JazzMan.bmp com a imagem obtida a partir do arquivo CUIF.3. Há perdas nos dados da imagem? Explique porque.

PSNR de aproximadamente 97.31591 dB, há perda de dados na conversão de RGB para YCbCr.


#### Questão 5. Qual a taxa de compressão obtida com CUIF.4? ).

CUIF.1 / CUIF.4 = Aproximadamente 1,70796:1

#### Questão 6. Indique a PSNR das codificações CUIF.4. Compare com a PSNR do CUIF3 e justifique.

PSNR de CUIF.4 com CUIF.1 aproximadamente: 152.48908 dB
PSNR de CUIF.3 com CUIF.1 aproximadamente: 97.31591 dB
PSNR de CUIF.4 com CUIF.3 aproximadamente: 97.29134 dB

O PSNR de CUIF.4 com CUIF.1 é mais alto que o de CUIF.3 com CUIF.1. Isso indica que os bits do CUIF.4 tendem a divergir menos dos bits do CUIF.1. Foi analisado que o uso de RLE, que tem perdas, reduziu a divergência dos bits (relativo aos bits originais) que surgiram na conversão de RGB para YCbCr.

#### Questão7. Codifiquem as imagens JazzMan.bmp e lena.bmp usando CUIF.4. Qual imagem obteve maior compressão? Explique porque.

taxa de compressão de lena.bmp usando CUIF.4 é aproximadamente: 1,02945:1

taxa de compressão de JazzMan.bmp usando CUIF.4 é aproximadamente: 1,70796:1

A imagem JazzMan.cuif teve uma compressão maior devido a alta quantidade de píxels repetidos.

