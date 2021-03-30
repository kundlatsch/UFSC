# INE5430 - Trabalho Prático 5

## Execução Localmente

Para executar o projeto localmente, as dependências podem ser instaladas utilizando o `Poetry`. Caso não possua ele instalado, o [tutorial da documentação do Poetry](https://python-poetry.org/docs/) pode ser seguido.
Tendo o Poetry instalado, basta rodar o seguinte comando para instalar as dependências:

`poetry install`

Para rodar o notebook pelo poetry, basta executar o comando:

`poetry run jupyter lab`

O notebook está presente no projeto em `/nn/assignment.ipynb`, caso deseje executá-lo de outra maneira.

Além disso, é necessário adicionar os dados (os arquivos fashion-mnist_train.csv e fashion-mnist_test.csv) na pasta `/nn/data`.

## Execução em Núvem

Uma versão na núvem do projeto, hospedado no Colab Notebooks, está disponível [nesse link](https://colab.research.google.com/drive/1st-wY2rM-H1IWLdWYEXSeBhKVPUAFzUO?usp=sharing).
O Colab não guarda os dados adicionados, portanto é preciso fazer upload dos arquivos fashion-mnist_train.csv e fashion-mnist_test.csv para dentro da pasta `data`,
utilizando o menu lateral esquerdo (arquivos).
