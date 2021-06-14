# INE5420 - Computação Gráfica

## Descrição

Este repositório contém o trabalho prático desenvolvido para a primeira parte da matéria INE5420 (Computação Gráfica) cursado no Departamento de Informática e Estatística da Universidade Federal de Santa Catarina. Ele consiste em um sistema gráfico interativo, capaz de representar, em perspectiva realista, objetos em 3D como modelos de arame e também como superfícies bicúbicas renderizadas como malhas de curvas.

## Atenção

Os detalhes sobre as implementações passadas estão no arquivo OLD_README.md. Decidimos deixar neste arquivo apenas as novidades desenvolvidas.

## Versão 1.9: Desenhando Superfícies Bicúbicas de Bézier e Spline

* Para esta entrega, foi adicionada uma nova classe ao projeto: `BicubicSurface`, que também herda de `Curve` (`app/wireframe.py`). Para construção de seu objeto, é necessário um parâmetro a mais: o tipo de curva. O tipo de curva pode ser tanto `Bezier` como `B-Spline`. Além disso, é necessário incluir 16 pontos, para ambos os casos. Também inserimos uma nova aba na interface de adição de `wireframes` (botão `Add`), em que é possível selecionar qual tipo de superfície bicúbica, assim como a acurácia desejada.

* Para os cálculos, foram incluidas as matrizes de `Bezier` e o vetor necessário, além de uma função que realiza as multiplicações necessárias: `calculate_curve_points_to_surface` (`app/math_functions.py`). Já na construção do objeto `BicubicSurface`, realizamos a obtenção das matrizes `Gbx, Gby` e `Gbz`, assim como calculamos os pontos necessários a partir dos parâmetros `s` e `t`, igualmente espaçados pela acurácia, entre [0, 1].

* Clipping, transformações, rotações, etc, funcionando para ambas as superfícies. Os pontos utilizados para testar foram os obtidos na aula de exemplo numérico, mas multiplicados por 20. Caso seja do interesse usá-los, pontos: [(0, 0, 0), (0, 60, 80), (0, 120, 60), (0, 200, 0), (60, 50, 40), (40, 120, 100), (60, 160, 100), (80, 0, 40), (120, 60, 40), (160, 120, 100), (140, 200, 90), (120, 0, 50), (200, 0, 0), (210, 60, 80), (210, 120, 60), (200, 180, 0)].


## Execução

Dependências diretas consistem nas bibliotecas `PyQt5` e `numpy`.

### Com poetry

Utilizamos `Poetry` para gerenciar dependências e ambientes. Para executar o projeto localmente, as dependências podem ser instaladas utilizando o `Poetry`. Caso não possua ele instalado, o [tutorial da documentação do Poetry](https://python-poetry.org/docs/) pode ser seguido.
Tendo o Poetry instalado, basta rodar o seguinte comando para instalar as dependências:

`poetry install`

É necessário ainda o pacote do `Qt`, ferramenta utilizada para o desenvolvimento da interface. Instruções de como instalá-lo em diversas distribuições podem ser encontradas na [Qt Wiki](https://wiki.qt.io/Main).

O projeto pode ser executado com ajuda do _makefile_ com o comando `make app`, e os testes podem ser rodados com `make test`.

### Sem poetry

Para executar sem a ajuda do __makefile__ e do `poetry`, basta rodar com

`python3 -m app`

atentando-se com as dependências necessárias.



