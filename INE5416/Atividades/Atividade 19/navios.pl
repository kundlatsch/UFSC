nacionalidade(brasileiro).
nacionalidade(espanhol).
nacionalidade(grego).
nacionalidade(ingles).
nacionalidade(frances).

saida(5).
saida(6).
saida(7).
saida(8).
saida(9).

carregamento(arroz).
carregamento(cacau).
carregamento(cha).
carregamento(milho).
carregamento(cafe).

chamine(azul).
chamine(branca).
chamine(verde).
chamine(vermelha).
chamine(preta).

destino(hamburgo).
destino(macau).
destino(manila).
destino(santos).
destino(rotterdam).


%X está à ao lado de Y
aoLado(X,Y,Lista) :- nextto(X,Y,Lista);nextto(Y,X,Lista).
                       
%X está à esquerda de Y (em qualquer posição à esquerda)
aEsquerda(X,Y,Lista) :- nth0(IndexX,Lista,X), 
                        nth0(IndexY,Lista,Y), 
                        IndexX < IndexY.
                        
%X está à direita de Y (em qualquer posição à direita)
aDireita(X,Y,Lista) :- aEsquerda(Y,X,Lista). 

%X está no canto se ele é o primeiro ou o último da lista
noCanto(X,Lista) :- last(Lista,X).
noCanto(X,[X|_]).

todosDiferentes([]).
todosDiferentes([H|T]) :- not(member(H,T)), todosDiferentes(T).

solucao(ListaSolucao) :- 

    ListaSolucao = [
        navio(Nacionalidade1, Saida1, Carregamento1, Chamine1, Destino1),
        navio(Nacionalidade2, Saida2, Carregamento2, Chamine2, Destino2),
        navio(Nacionalidade3, Saida3, Carregamento3, Chamine3, Destino3),
        navio(Nacionalidade4, Saida4, Carregamento4, Chamine4, Destino4),
        navio(Nacionalidade5, Saida5, Carregamento5, Chamine5, Destino5)
    ],

    %O navio Grego sai às 6 da manhã e carrega Café.
    member(navio(grego, 6, cafe, _, _), ListaSolucao),

    %O navio do meio tem a chaminé Preta.
    Chamine3 = preta,
    
    %O navio Inglês sai às 9 da manhã.
    member(navio(ingles, 9, _, _, _), ListaSolucao),
    
    %O navio Francês, que tem a chaminé Azul, está à esquerda do navio que carrega Café
    aEsquerda(navio(frances, _, _, azul, _), navio(_, _, cafe, _, _), ListaSolucao),
    
    %À direita do navio que carrega Cacau está o navio que vai para Macau.
    aEsquerda(navio(_, _, cacau, _, _), navio(_, _, _, _, macau), ListaSolucao),
    
    %O navio Brasileiro está indo para Manila.
    member(navio(brasileiro, _, _, _, manila), ListaSolucao),
    
    %O navio que carrega Arroz está ancorado ao lado do navio com chaminé Verde.
    aoLado(navio(_, _, arroz, _, _), navio(_, _, _, verde, _), ListaSolucao),
    
    %O navio que vai para Santos sai às 5 da manhã.
    member(navio(_, 5, _, _, santos), ListaSolucao),
    
    %O navio Espanhol sai às 7 da manhã e está à direita do navio que vai para Macau.
    aDireita(navio(espanhol, 7, _, _, _), navio(_, _, _, _, macau), ListaSolucao),
    
    %O navio com a chaminé Vermelha vai para Hamburgo.
    member(navio(_, _, _, vermelha, hamburgo), ListaSolucao),
    
    %O navio que sai às 7 da manhã está ao lado do navio que tem a chaminé Branca.
    aoLado(navio(_, 7, _, _, _), navio(_, _, _, branca, _), ListaSolucao),
    
    %O navio do canto carrega Milho.
    noCanto(navio(_, _, milho, _, _),ListaSolucao),
    
    %O navio com chaminé Preta sai às 8 da manhã
    member(navio(_, 8, _, preta, _), ListaSolucao),
    
    %O navio que que carrega Milho está ancorado ao lado do navio que carrega Arroz.
    aoLado(navio(_, _, milho, _, _), navio(_, _, arroz, _, _), ListaSolucao),
    
    %O navio que vai para Hamburgo sai às 6 da manhã.
    member(navio(_, 6, _, _, hamburgo), ListaSolucao),
    
    %Testa todas as possibilidades...
    nacionalidade(Nacionalidade1), nacionalidade(Nacionalidade2), nacionalidade(Nacionalidade3), nacionalidade(Nacionalidade4), nacionalidade(Nacionalidade5),
    todosDiferentes([Nacionalidade1, Nacionalidade2, Nacionalidade3, Nacionalidade4, Nacionalidade5]),
    
    saida(Saida1), saida(Saida2), saida(Saida3), saida(Saida4), saida(Saida5),
    todosDiferentes([Saida1, Saida2, Saida3, Saida4, Saida5]),
    
    carregamento(Carregamento1), carregamento(Carregamento2), carregamento(Carregamento3), carregamento(Carregamento4), carregamento(Carregamento5),
    todosDiferentes([Carregamento1, Carregamento2, Carregamento3, Carregamento4, Carregamento5]),
    
    chamine(Chamine1), chamine(Chamine2), chamine(Chamine3), chamine(Chamine4), chamine(Chamine5),
    todosDiferentes([Chamine1, Chamine2, Chamine3, Chamine4, Chamine5]),
    
    destino(Destino1), destino(Destino2), destino(Destino3), destino(Destino4), destino(Destino5),
    todosDiferentes([Destino1, Destino2, Destino3, Destino4, Destino5]).
    
    
