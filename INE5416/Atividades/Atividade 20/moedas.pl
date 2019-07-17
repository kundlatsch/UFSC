:- dynamic minimoMoedas/2. %define que fatos/regras na forma minimoMoedas(_,_) serão dinâmicos
:- retractall( minimoMoedas(_,_) ). %apaga quaisquer fatos/regras na forma minimoMoedas(_,_) previamente registrados
    
moeda(20).
moeda(30).
moeda(60).
%encontrar moedas para 110, deve ser 3

%moeda(1).
%moeda(5).
%moeda(6).
%moeda(8).
%encontrar moedas para 11, deve ser 2

%moeda(35).
%moeda(150).
%encontrar moedas para 1000, deve ser 22
%encontrar moedas para 2000, deve ser 21
%encontrar moedas para 2100, deve ser 14

minimoMoedas(N,1) :- moeda(N), !.
minimoMoedas(N,K) :-
    N > 0,
    findall(KResto, (moeda(X), X < N, Resto is N - X, minimoMoedas(Resto, KResto)), ListaKResto), %Tenta descobrir qual é a última moeda usada para gerar o valor N
    min_list(ListaKResto, KMinResto), %Pega a que usar o menor número de moedas
    K is KMinResto + 1, %Soma 1 no total de moedas usadas
    asserta(minimoMoedas(N,K) :- !). %Salva o resultado


