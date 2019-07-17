% Questão 1

posicao(X, [X|_],  1) :- !.
posicao(X, [_|T], P) :- posicao(X, T, PR), P is PR + 1.

/*
	?- posicao(3, [2, 3, 4], P).
	P = 2.

	?- posicao(4, [2, 3, 4], P).
	P = 3.
*/

% Questão 2

% inserirElementoPosicao(X,P,L1,L2) :- 

% Questão 3

numerosParaPalavras([], []).
numerosParaPalavras([1|T], L2) :- numerosParaPalavras(T, LX), L2 = ["um"		|LX].
numerosParaPalavras([2|T], L2) :- numerosParaPalavras(T, LX), L2 = ["dois"		|LX].
numerosParaPalavras([3|T], L2) :- numerosParaPalavras(T, LX), L2 = ["tres"		|LX].
numerosParaPalavras([4|T], L2) :- numerosParaPalavras(T, LX), L2 = ["quatro"	|LX].
numerosParaPalavras([5|T], L2) :- numerosParaPalavras(T, LX), L2 = ["cinco"		|LX].
numerosParaPalavras([6|T], L2) :- numerosParaPalavras(T, LX), L2 = ["seis"		|LX].
numerosParaPalavras([7|T], L2) :- numerosParaPalavras(T, LX), L2 = ["sete"		|LX].
numerosParaPalavras([8|T], L2) :- numerosParaPalavras(T, LX), L2 = ["oito"		|LX].
numerosParaPalavras([9|T], L2) :- numerosParaPalavras(T, LX), L2 = ["nove"		|LX].
numerosParaPalavras([0|T], L2) :- numerosParaPalavras(T, LX), L2 = ["zero"		|LX].

/*
	?- numerosParaPalavras([4,2], A).
	A = ["quatro", "dois"] .

	?- numerosParaPalavras([0,8,9], A).
	A = ["zero", "oito", "nove"] .
*/

% Questão 4

soma([], X) :- X is 0.
soma([H|T], X) :- soma(T, Y), X is H + Y.

/*
	?- soma([1,2,3,4],A).
	A = 10.

	?- soma([10,0,9,1],A).
	A = 20.
*/

% Questão 5

% Regra auxiliar para computar número de elementos na lista
elementos([], X) :- X is 0.
elementos([_|T], X) :- elementos(T, Y), X is Y + 1.

media(L,X) :- soma(L, S), elementos(L, E), X is S / E.

/*
	?- media([2,2,2], A).
	A = 2.

	?- media([10,4,4], A).
	A = 6.
*/

% Questão 6

menor([H], X) :- X is H.
menor([H|T], X) :- menor(T, Y), ((H < Y, X is H); X is Y).

/*
	?- menor([5,2,4], X).
	X = 2 .

	?- menor([2,20,1], X).
	X = 1 .
*/

% Questão 7

palindrome(X) :- inverte(X,X).

/*
	?- palindrome([1,2,3,4]).
	false.

	?- palindrome([2,2,2,2]).
	true.
*/

% Questão 8

% Regra auxiliar para computar o maior elemento da lista
maior([H], X) :- X is H.
maior([H|T], X) :- maior(T, Y), ((H > Y, X is H); X is Y).

diferencaMaiorMenor([],X) :- X is 0.
diferencaMaiorMenor(L,X) :- maior(L, A), menor(L, B), X is A - B. 

/*
	?- diferencaMaiorMenor([4,2,1,10], A).
	A = 9 .

	?- diferencaMaiorMenor([], A).
	A = 0 .
*/

% Questão 9

ocorrencias([], _, N) :- N is 0.
ocorrencias([H|T], X, N) :- (H =:= X, ocorrencias(T, X, R), N is R + 1); (ocorrencias(T, X, R), N is R).

/*
	?- ocorrencias([2, 2, 1], 2, A).
	A = 2 .

	?- ocorrencias([4, 2, 1], 2, A).
	A = 1 .

	?- ocorrencias([5, 2, 5, 2, 5, 10, 5, 9, 0, -1, 5], 5, A).
	A = 5 .
*/

% Questão 10

inverte(X,Y) :- inverte(X,[],Y).

inverte([],A,A).
inverte([H|T],R,A) :- inverte(T,[H|R],A).

/*
	?- inverte([1,2,3],A).
	A = [3, 2, 1].

	?- inverte([4,2,3,2,3,4],A).
	A = [4, 3, 2, 3, 2, 4].

	?- inverte([4,1,1,0,0],A).
	A = [0, 0, 1, 1, 4].
*/

% Questão 11

% Regra auxiliar para inserir elemento na lista
inserirElemento(X,L,[X|L]).

% Regra auxiliar para fazer a chamada de primeiros com uma lista auxiliar temporária vazia
primeiros(N, L1, L2) :- primeiros(N, L1, L2, []).

primeiros(N, [H|T], L2, LA) :- 
	(N =:= 1, inserirElemento(H, LA, LR), inverte(LR, LF), L2 = LF);
	(inserirElemento(H, LA, LR), primeiros(N-1, T, L2, LR)).

/*
	?- primeiros(2, [1, 2, 3, 4], A).
	A = [1, 2] .

	?- primeiros(4, [5, 2, 3, 4, 6, 9], A).
	A = [5, 2, 3, 4] .
*/

% Questão 12

apagar(_, [], []).
apagar(N,[_|T],L2) :- (N =:= 1, L2 = T); apagar(N-1, T, L2).

/*
	?- apagar(2, [1,2,3,4], A).
	A = [3, 4] .

	?- apagar(4, [1,2,3,4,5,6,7], A).
	A = [5, 6, 7] .

	?- apagar(5, [1,2,3,4], A).
	A = [] .
*/

% Questão 13

% E is elementos(L), M is mod(E,2), =:= 0,

dividir(L, A, B) :- elementos(L,E), M is mod(E,2), M =:= 0, corte(L, L, A, B).

corte(B, [], [], B).

corte([H|T], [_, _|T1], [H | T2], B) :-
    corte(T, T1, T2, B).

/*
	?- dividir([1,2,3,4],A,B).
	A = [1, 2],
	B = [3, 4] .

	?- dividir([1,2,3,4,5],A,B).
	false.
*/

% Questão 14

uniao([],[],[]).
uniao(List1,[],List1).
uniao(List1, [Head2|Tail2], [Head2|Output]):-
    \+(member(Head2,List1)), uniao(List1,Tail2,Output).
uniao(List1, [Head2|Tail2], Output):-
    member(Head2,List1), uniao(List1,Tail2,Output).

/*
	?- uniao([1,2,3],[4,5,6],A).
	A = [4, 5, 6, 1, 2, 3] .

	?- uniao([1,2,3],[4,5,1],A).
	A = [4, 5, 1, 2, 3] .
*/

% Questão 15

diferenca(L,[],L).
diferenca(L,[H|T],R) :- select(H,L,R0), diferenca(R0,T,R).

/*
	?- diferenca([1,2,3],[1,2],A).
	A = [3] .

	?- diferenca([1,2,3,5,9],[5,2],A).
	A = [1, 3, 9] .
*/