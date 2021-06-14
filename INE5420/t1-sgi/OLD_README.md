## Versão 1.8: Projeção em perspectiva

* Não foi implementada.

## Versão 1.7: Objetos 3D e Projação Paralela Ortogonal

* Para esta tarefa, atualizamos a classe `Wireframe` para suportar pontos, linhas e objetos 3D. Desse modo, tudo que for inserido, na verdade, é tratado como 3D: objetos 2D são objetos 3D com y = 0, definido como padrão na tela de inserção. Para isso, todas as funções que geram matrizes de transformações (translação, escalonamento e rotação) foram atualizadas para 3D (`app/math_functions`). No caso da rotação, gera-se três matrizes e retorna a composição delas.

* A Projeção Paralela Ortogonal está funcionando depois das alterações mencionadas. Além disso, tivemos que adaptar o leitor de arquivos `.obj`. Assim, para agrupamentos, definidos por "g", eles compõem um objeto adicional chamado `WireframeGroup`, responsável por centralizar as transformações. A definição de múltiplos "o" por arquivo ainda é permitida.

* Foram adicionados diversos exemplos na pasta `obj/3d`. retirados mo dodle da disciplina e dos seguintes sites: [https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html] e [http://paulbourke.net/dataformats/obj/]. Não produzimos nenhum 3D. Os objetos 2D ainda funcionam normalmente.

* A **transformação** de rotação ao redor do centro do objeto está com problemas. Entretanto, o resto funciona normalmente.


## Versão 1.6: B-Splines utilizando Forward Differences

* Impementamos B-Splines utilizando Foward Differences. Para isso, nós criamos uma classe `Curve`, que herda de `Wireframe`. Após essa mudança, tanto `BezierCurve` como `BSplineCurve` herdam de `Curve`. Para `BSplineCurve`, existem algumas funções importantes de nota:
    - `build_curve_coordinates`: as coordenadas são criadas na própria função . Nela, é invocada a função de gerar as diferenças iniciais para cada conjunto de 4 pontos de controle e realiza as iterações necessárias para o algoritmo.
    - Em `app/math_functions.py`, estão as funções auxiliares , que retorna a matriz de B-Spline; `calculate_initial_differences`, que calcula o vetor de diferenças iniciais; `calculate_bspline_parameters`, que agrega as duas funções anteriores.
    - o `clipping` segue a mesma orientação dea entrega anterior.
    
* Mudamos a interface de criação de curvas para selecionar qual o modelo desejado, `Bezier` ou `B-Spline`. Conforme seleção, a acurácia (delta, no caso da `B-Spline`) padrão é alterada (20 para a primeira, 0.1 para a segunda). Quando inseridos no `display file`, cada curva específica tem seu nome especificado, também.

## Versão 1.5: Implemente Curvas em 2D usando Blending Functions

* Nesta entrega, decidimos implementar a Curva de Bézier. Para isso:
    - adicionamos uma nova classe `BezierCurve`, que herda de `Wireframe`. Ela possui duas alterações básicas: recebe um conjunto de `base points`, que necessariamente respeita as regras de criação (tamanho 4, 7, 10...), e um método novo que cria as coordenadas baseadas nos `base points` e na `accuracy` definida. Sendo assim, é possível inserir diversas curvas em apenas um objeto `BezierCurve`, como solicitado, com continuidade G(0).
    - foi alterada a interface de criação de `wireframe`: agora, há uma nova aba destinada para a inserção de Curva de Bézier (botão `add`).
    - definimos as `blending functions` de Bézier em `app/math_functions.py`, assim como o método auxiliar para o cálculo dos pontos.
    - aplicamos o `clipping` de pontos, na função `clip` (`app/clipping.py`).

* Como sugestão de teste, criamos uma figura de um coração, composta por duas curvas de Bézier. Os pontos base, caso queiram visualizar, são (0.0, 100.0), (120.0, 200.0), (240.0, 100.0), (0.0, -100.0), para o lado direito, e (0.0, 100.0), (-120.0, 200.0), (-240.0, 100.0), (0.0, -100.0) para o esquerdo. É interessante notar que o `clipping` funciona normalmente caso alguma das extremidades seja levada para fora do retângulo, no sentido de aparecer a "volta" do objeto em tela.

## Versão 1.4: Clipping e preenchimento de polígonos

* O sistema foi adaptado para suportar o `clipping` de objetos no mundo. Para isso, foi fixado, como solicitado, um retângulo vermelho com proporções menores que as do Canvas utilizados (20px). Atualmente, o sistema suporta o `clipping` para pontos, linhas e polígonos. Como o sistema utiliza Sistema de Coordenadas Normalizado, o `clipping` se baseou nos limites de [-1, 1], em ambos os sentidos. Os algoritmos estão no arquivo `app/clipping.py`. Um selecionador de configuração de `clipping` foi inserido na parte inferior esquerda da interface.
    - Para linhas, foram implementados os algoritmos de `Cohen-Sutherland` e `Liang-Barsky`.
    - Para polígonos, foi implementado o algoritmo `Weiler-Atherton`.

* Para facilitar a verificação, além da opção por selecionar entre os dois algoritmos de linha, foi adicionado a opção `no-clipping`, com intuito de permitir a observação do "vazamento" de pontos/linhas para fora do retângulo fixado. **Sempre** que um algoritmo de linha é selecionado, automaticamente são ligados os algoritmos de clipping de pontos e polígonos.

* Foram adicionados 6 novos exemplos de arquivos `.obj` para facilitar a verificação, também. A navegação (cima, baixo, esquerda, direita) é encorajada.
    - `clipping_points_example.obj`: possui pontos em diversas localizações da window, dentro e fora do retângulo de referência.
    - `clipping_line_example.obj`: possui um conjunto de linhas com diversos tipos de intersecções com bordas e vazamentos.
    - `wireframe_clipping_{0-4}.obj`: polígono de exemplo, transladado e rotacionado para as quatro intersecções principais do retângulo de referência. É interessante abrir os quatro ao mesmo tempo, ligar e desligar o `clipping`.


* Foi adicionada a opção de preencher o polígono com cor, no momento de sua criação. 



## Versão 1.3: Implementação de Rotação da Window, SCN, leitura e escrita de arquivos obj

* O sistema de coordenadas foi alterado para a utilização do Sistema de Coordenadas Normalizado. Para isto, foram feitas as seguintes modificações:
    - A window foi fixada entre [-1, 1].
    - Criação da função `build_window_normalizations` (`app/math_functions`), que realiza a criação das matrizes de translação ao centro da window, a rotação do ângulo inserido e o escalonamento para a normalização. As matrizes, portanto, são compostas e retornadas. Elas são recalculadas sempre que necessário, mas apenas uma vez, pelo controlador da `main_window`. Sendo assim, cada `wireframe` recebe pronta a matriz com as transformações de window.
    - Na parte de transformações de coordenadas dos `wireframes` (`transform_coordinates` em `app/wireframe.py`), é feito um passo antes de compor as matrizes de transformações, multiplicando as coordenadas originais e homogêneas pela matriz de normalização da window. As coordenadas normalizadas e transformadas são salvas e utilizadas no desenho.

* Para a rotação da window, foi habilitado o botão de rotação "⮏", que irá girar para a esquerda o valor de graus informado (para a direita, basta inverter o sinal). Com isso, fará que recalcule a matriz de normalização, assim como as funções de navegação normais (que estão funcionando!!).

* Para lidar com arquivos `.obj`, foram criadas duas classes: `ObjLoader` e `ObjWriter`(`app/obj_handler`), além de serem adicionados dois botões: `Save` e `Load`.
    - `Load`: carrega os objetos de um aquivo obj para a cena. Caso um arquivo obj especifique um `.mtl`, é preciso que ele se encontre na mesma pasta do obj. 
    - `Save`: exporta todos os `wireframes` para um arquivo obj com o nome especificado para a cena, criando um arquivo `.mtl` auxiliar para salvar as cores dos objetos. Como as coordenadas transformadas estão normalizadas, é necessário realizar um passo de desnormalização para salvá-las. Para isso, cria-se uma matriz de normalização inversa e aplica-se as coordenadas normalizadas.

* Três arquivos foram disponibilizados, na pasta `/obj`, como exemplo de carregamento e exportação de `.obj`: `house.obj`, `pentagram.obj` e `ruby.obj`, todos eles criados e exportados pelo próprio programa.

## Versão 1.2: Implementação de Transformações 2D e Coordenadas Homogêneas

* Foi adicionado um novo botão, `Transform`, que abre uma janela de diálogo referente ao objeto selecionado no `display file`.

* A janela de transformações consta com 4 abas: `Rotation`, `Translation`, `Scaling` e `Reflection`, além da lista de transformações do objeto. O objeto é atualizado visualmente a cada transformação adicionada.

    - `Rotation`: é possível selecionar entre os três tipos: ao redor do centro, origem e de um ponto arbitrário.
    - `Translation`: definir (x, y).
    - `Scaling`: definir (x, y).
    - `Reflection`: escolher entre X, Y e origem.

* As matrizes de transformações estão definidas em `app/math_functions.py`, cada qual com exemplo de saída. Internamente, definimos códigos para traduzir as transformações e ser possível realizar os cálculos.

* `Wireframes` agora possuem um conjunto de coordenadas homogêneas, de coordenadas transformadas e de transformações. A função que calcula as transformações está em `app/wireframe.py`: `transform_coordinates`.

    - Coordenadas homogêneas são utilizadas nos cálculos das transformações.
    - Coordenadas transformadas são as que passaram pelas transformações e serão mostrada em tela.
    - A função `transform_coordinates` itera sobre as coordenadas homogêneas, aplicando todas as transformações e atualizando-se. Casos que precisam de translação são tratados ali mesmo. (Uma versão simplificada dessa função consta em `apply_transformations_to_points`, utilizada para testar apenas as transformações)

* Foram adicionados testes para alguns aspectos do programa. Em `tests/test_wiframe.py` são testados os exercícios de transformações (1.2.2).

## Versão 1.1: Sistema Básico com Window e Viewport

* Os objetos são definidos como `wireframes` (`wireframe.py`), tendo seu tipo decidido dinamicamente através do enum `Shape` (`utils.py`). `Wireframes` possuem nome, tipo, lista de coordenadas, número de pontos e cor de exibição.

* A tela principal apresenta as funcionalidades do Sistema Básico. À esquerda, está localizada a lista de objetos e botões para manipulação.
    - `Add`: abre uma nova janela para a criação de um novo `wireframe`. Nesta janela, é necessário informar os pontos desejados. Também é possível selecionar a cor de exibição.
    - `Delete`: deleta o `wireframe` selecionado na lista de objetos.
    - `Clear`: deleta todos os `wireframes` da lista de objetos.
    - `Refresh`: reseta os parâmetros de navegação e redesenha os objetos na tela.

* Ainda à esquerda, estão disponíveos os botões de navegação 2D e zoom, com `step` previamente definidos:
    - `Navegação 2D`: esquerda, direita, cima e baixo.
    - `Zoom`: in e out.

* A tela de criação de `wirefame` apresenta os botões:
    - `Add`: adiciona o ponto (X, Y) descrito no campo de texto acima.
    - `Delete`: deleta o ponto selecionado da lista de pontos à direita.
    - `Pick Color`: selecionar cor de exibição do `wireframe`.
    - `Draw`: desenha o `wireframe` no canvas.