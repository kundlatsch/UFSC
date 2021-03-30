# A Identificação de Objetos no RX do Aeroporto

Reconhecimento de padrões é o processo de treinamento de uma rede neural para atribuir uma classe correta para um conjunto de padrões de entrada. Uma vez treinada, a rede pode ser usada para classificar padrões que não tenha visto antes.
 
Neste trabalho, um conjunto de dados deve ser usado para treinar e testar uma rede neural, que classifica  objetos de vestuário capturados por uma máquina de RX no aeroporto. Os dados para treinamento, validação e teste foram obtidos a partir de uma base de treinamento do kaggle (www.kaggle.com).

As imagens já foram previamente tratadas e centralizadas. Cada imagem possui 28x28 pixels totalizando 784 pixels em escala de cinza. 

Existem 60000 imagens para treinamento (você deve separar em treinamento e validação) que estão disponíveis no arquivo fashion-mnist_train.csv. A primeira linha do arquivo tem os labels e as linhas seguintes as imagens propriamente ditas. A primeira coluna de cada linha tem a classificação correta da imagem, cujo código pode ser visto abaixo.

0 'T-shirt/top';
1 'Trouser';
2 'Pullover';
3 'Dress';
4 'Coat';
5 'Sandal';
6 'Shirt';
7 'Sneaker';
8 'Bag';
9 'Ankle boot'

Este exercício pode ser feito em qualquer linguagem/ambiente mas se for feito em OCTAVE/MATLAB, foi disponibilizada um script função chamada prepData.m que importa os dados dos arquivos de treinamento e teste  e permite a visualização de alguns artigos.  

Além da demonstração da rede neural funcionando, um Relatório completo deve ser entregue, contendo informações como: Como os dados foram normalizados; Como foi separado o conjunto de  treinamento e de teste;  Qual a arquitetura de rede utilizada; Quantos e Quais experimentos foram feitos até chegar no resultado final; Como foi o treinamento; Qual a taxa de acertos da rede; A matriz de confusão; Exemplos de objetos que foram mal-classificados pela Rede; Fatos que você achou interessante; etc...

OBSERVAÇÃO: Em aula nós aprendemos sobre rede de camada rasa (1 ou 2 camadas escondidas). Se você quiser resolver este problema usando redes de camada profunda (deep learning) e redes convolucionais, você pode, mas TEM DE RESOLVER TAMBÉM usando as REDES DE CAMADA RASA.