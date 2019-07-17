% Questão 3

divisivel(N, K) :- mod(N, K) =:= 0.

/*
	?- divisivel(10, 5).
	true.

	?- divisivel(10, 4).
	false.
*/

% Questão 4

triangulo(X, Y, Z) :- verificar(X, Y, Z), verificar(X, Z, Y), verificar(Y, Z, X).
verificar(N1, N2, N3) :- N1 - N2 < N3, N3 < N1 + N2.

/*
	?- triangulo(4, 8, 9).
	true.

	?- triangulo(10, 5, 4).
	false.
*/

% Questão 5

eqSegundoGrau(A,B,C,R1,R2)  :- 	D is ((B*B)-(4*A*C)),         
								D >= 0,         
								R1 is ((-B+sqrt(D))/2*A),         
								R2 is ((-B-sqrt(D))/2*A).

% Questão 6

potencia(X,Y,Resultado) :- Resultado is X ^ Y.

/*
	?- potencia(2, 3, 8).
	true.

	?- potencia(2, 4, 8).
	false.
*/

% Questão 7

absoluto(N, X) :- (N >= 0, X is N); (N < 0, X is N * -1).

/*
	?- absoluto(-3, X).
	X = 3.

	?- absoluto(10, X).
	X = 10 .
*/

% Questão 8

areaTriangulo(B,A,Area) :- Area is B * A / 2.

/*
	?- areaTriangulo(4,2,Area).
	Area = 4.

	?- areaTriangulo(10,5,Area).
	Area = 25.
*/

% Questão 9

meuAnd(A,B) :- A,B.
meuOr(A,B) :- A;B.
meuNand(A,B) :- not(meuAnd(A,B)).
meuXor(X,Y) :- meuOr(X,Y), meuNand(X,Y).

/*
	?- meuXor(true, false).
	true .

	?- meuXor(true, true).
	false.

	?- meuXor(false, true).
	true.

	?- meuXor(false, false).
	false.
*/

% Questão 10

aprovado(A, B, C) :- (A + B + C) / 3 >= 6.

/*
	?- aprovado(10, 5, 6).
	true.

	?- aprovado(2, 5, 6).
	false.
*/

% Questão 11

fib(1, 1) :- !.
fib(2, 1) :- !.
fib(N, K) :-
        N > 1,
        N1 is N-1,
        N2 is N-2,
        fib(N1, K1),
        fib(N2, K2),
        K is K1+K2.

/*
	?- fib(5, A).
	A = 5.

	?- fib(10, A).
	A = 55.
*/

% Questão 12

% Questão 13

maior(A, B, C, Y) :- 
				(A >= B, A >= C, Y is A);
				(B >= A, B >= C, Y is B);
				(C >= A, C >= B, Y is C).

/*
	?- maior(1, 2, 3, A).
	A = 3.

	?- maior(1, 2, 2, A).
	A = 2 .
*/

% Questão 14

operacao(Op, X, Y, Resultado) :-
   				Eq=..[Op, X, Y],
			 	Resultado is Eq.

/*
	?- operacao(+, 4, 5, A).
	A = 9.

	?- operacao(/, 180, 5, A).
	A = 36.
*/

% Questão 15

euclid(A, 0, Z) :- Z is A.
euclid(A, B, Z) :- B > A, euclid(B, A, Z).
euclid(A, B, Z) :- X is A mod B, euclid(B, X, Z).
mdc(A, B, Resultado) :- euclid(A, B, Resultado).

/*
	?- mdc(15, 20, A).
	A = 5 .

	?- mdc(4, 20, A).
	A = 4 . 
*/

% Questão 16

% Questão 17

mmc(X, Y, Resultado) :- mdc(X, Y, MDC), Resultado is X * Y // MDC.

/*
	?- mmc(21, 6, A).
	A = 42 .

	?- mmc(8, 2, A).
	A = 8 .
*/

% Questão 18

coprimos(X, Y) :- mdc(X, Y, Resultado), Resultado =:= 1;.

% Questão 19

totient_phi(1, 1).
totient_phi(M, P) :-
	M2 is M - 1,
	tphi(M, M2, P).
tphi(_, 0, 0).
tphi(M, R, P) :-
  	coprimo(M, R),
  	R2 is R - 1,
  	tphi(M, R2, P2),
  	P is P2 + 1.
tphi(M, R, P) :-
  	\+ coprimo(M, R),
  	R2 is R - 1,
	tphi(M, R2, P).

% Questão 20

divisivel(X,Y) :- 0 is X mod Y, !.

divisivel(X,Y) :- X > Y+1, divisivel(X, Y+1).

primo(2) :- true,!.
primo(X) :- X < 2,!,false.
primo(X) :- not(divisivel(X, 2)).

/*
	?- primo(5).
	true.

	?- primo(10).
	false.
*/