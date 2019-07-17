% Relacao de filmes
%    filme(id, titulo, ano, diretor, nacionalidade).
%
filme(f1, 'Monty Python: O Sentido da Vida', 1983, 'Terry Jones', uk).
filme(f2, 'Edukators', 2004, 'Hans Weingartner', de).
filme(f3, 'Lavoura Arcaica', 2001, 'Luiz Fernando Carvalho', br).
filme(f4, 'Lisbela e o Prisioneira', 2003, 'Guel Arraes', br).
filme(f5, 'Abril despedaçado', 2001, 'Walter Salles', br).
filme(f6, 'Diários de motocicleta', 2004, 'Walter Salles', br).

% Relacao de paises
%     pais(sigla, nome).
%
pais(uk, 'Unided Kingdom').
pais(de, 'Alemanha').
pais(br, 'Brasil').

% Relacao de DVD (a caixa em si)
%     dvd(id do DVD, id do filme, estante).
%
dvd(d1, f1, est1).
dvd(d2, f2, est1).
dvd(d4, f4, est1).
dvd(d3, f3, est2).
dvd(d5, f5, est3).
dvd(d6, f1, est1).
dvd(d7, f2, est4).
dvd(d8, f2, est4).

% Relacao de clientes
%     cliente(cod, nome, telefone).
%
cliente(c1, 'Bob', '333-3112').
cliente(c2, 'Zeca', '245-1099').
cliente(c3, 'Tom', '323-0685').
cliente(c4, 'Bianca', '333-4391').
cliente(c5, 'Alice', '251-7439').
cliente(c6, 'Maria', '212-3271').

% Relacao de locacoes
%     locacao(cod cliente, nro do DVD, data de entrega)
%
locacao(c1, d1, '2005-11-07').
locacao(c1, d2, '2005-11-07').
locacao(c3, d5, '2005-11-09').
locacao(c2, d3, '2005-11-10').
locacao(c3, d3, '2005-11-11').
locacao(c4, d8, '2005-11-12').
locacao(c5, d7, '2005-11-12').
locacao(c6, d6, '2005-11-12').
locacao(c1, d5, '2005-11-13').
locacao(c1, d6, '2005-11-13').
locacao(c6, d2, '2005-11-14').
locacao(c3, d7, '2005-11-14').
locacao(c3, d8, '2005-11-14').
locacao(c5, d1, '2005-11-15').

% A

/*
	?- findall(A, (filme(_,A,G,_,_), G < 2001), L).
	L = ['Monty Python: O Sentido da Vida'].
*/

% B

/*
	?- findall(A, (filme(_,A,G,_,_), G > 2000, G < 2005), L).
	L = ['Edukators', 'Lavoura Arcaica', 'Lisbela e o Prisioneira', 'Abril despedaçado', 'Diários de motocicleta'].
*/

% C

/*
	?- findall(A, (filme(_,A,_,_,P), P == br), L), sort(L, LOrdenado).
	L = ['Lavoura Arcaica', 'Lisbela e o Prisioneira', 'Abril despedaçado', 'Diários de motocicleta'],
	LOrdenado = ['Abril despedaçado', 'Diários de motocicleta', 'Lavoura Arcaica', 'Lisbela e o Prisioneira'].
*/

% D

/*
	?- findall(A, (filme(_,A,_,_,P), P \== br), L), sort(L, LOrdenado).
	L = ['Monty Python: O Sentido da Vida', 'Edukators'],
	LOrdenado = ['Edukators', 'Monty Python: O Sentido da Vida'].
*/

% E

/*
	?- findall(A, (filme(_,A,G,_,P), P \== de, G > 2003), L), sort(L, LOrdenado).
	L = LOrdenado, LOrdenado = ['Diários de motocicleta'].
*/

% F

/*
	?- findall(R, (filme(F,_,_,_,P), dvd(R,F,_), P \== br), L).
	L = [d1, d6, d2, d7, d8].
*/

% G

/*
	?- findall(R, (filme(F,_,_,_,P), dvd(R,F,E), P == br, E == est2), L).
	L = [d3].
*/

% H

/*
	?- findall(D, (filme(F,_,_,D,_), dvd(_,F,E), E == est1), L), sort(L, LOrdenado). 
	L = ['Terry Jones', 'Terry Jones', 'Hans Weingartner', 'Guel Arraes'],
	LOrdenado = ['Guel Arraes', 'Hans Weingartner', 'Terry Jones'].
*/

% I

/*
	?- findall(T, (filme(F,T,A,D,N), not(dvd(I,F,_))),F).
	F = ['Diários de motocicleta'].
*/

% J

/*
	?- findall(N, (filme(F,T,A,D,N), (dvd(I,F,E)), (E == est1; E == est4)),F), sort(F,FOrdenado).
	F = [uk, uk, de, de, de, br],
	FOrdenado = [br, de, uk].
*/

% K

/*
	?- findall(N, (cliente(C, N, _), locacao(C, _, D), D == '2005-11-07'), L).
	L = ['Bob', 'Bob'].
*/

% L

/*
	?- findall(E, (locacao(_, D, _), dvd(D, _, E)), L), sort(L, LOrdenada).
	L = [est1, est1, est3, est2, est2, est4, est4, est1, est3|...],
	LOrdenada = [est1, est2, est3, est4].
*/

% M

/*
	?- findall(T, (cliente(C, N, _), locacao(C, D, _), dvd(D, F, _), filme(F, T, _, _, _), (N == 'Maria')), L).
	L = ['Monty Python: O Sentido da Vida', 'Edukators'].
*/

% N

/*
	?- findall(T, (cliente(C, N, _), locacao(C, D, _), dvd(D, F, _), filme(F, T, _, _, _), (N == 'Maria')), L), list_to_set(L, L2), length(L2, NumeroFilmes).
	L = L2, L2 = ['Monty Python: O Sentido da Vida', 'Edukators'],
	NumeroFilmes = 2.
*/

% O

/*
	?- bagof(T, IdF ^ _IdDVD ^ _A ^ _D ^ _N ^(dvd(_IdDVD, IdF, Est), filme(IdF, T, _A, _D, _N)) , Titulos).
	Est = est1,
	Titulos = ['Monty Python: O Sentido da Vida', 'Edukators', 'Lisbela e o Prisioneira', 'Monty Python: O Sentido da Vida'] ;
	Est = est2,
	Titulos = ['Lavoura Arcaica'] ;
	Est = est3,
	Titulos = ['Abril despedaçado'] ;
	Est = est4,
	Titulos = ['Edukators', 'Edukators'].
*/

% P

/*
	?- findall(NP, (filme(F, _T, _A, _D, N), pais(N, NP), dvd(IdDVD, F, _Est), locacao(C, IdDVD, Data), cliente(C, NomeCliente, _Telefone), NomeCliente = 'Bob', Data = '2005-11-07'), L).
	L = ['Unided Kingdom', 'Alemanha'].
*/

% Q

/*
	findall(NP, (filme(F, _T, _A, _D, N), pais(N, NP), dvd(IdDVD, F, _Est), locacao(C, IdDVD, Data), cliente(C, NomeCliente, _Telefone), NomeCliente = 'Bob', NP = 'Brasil'), NPs), length(NPs, L).
	NPs = ['Brasil'],
	L = 1.
*/

% R

/*
	?- setof(T, F ^ _A ^ _D ^ _N ^ IdDVD ^ Est ^ F ^ C ^_Data ^ _Tel ^ (filme(F, T, _A, _D, _N), dvd(IdDVD, F, Est), locacao(C, IdDVD, _Data), cliente(C, Cliente, _Tel)), Filmes).
	Cliente = 'Alice',
	Filmes = ['Edukators', 'Monty Python: O Sentido da Vida'] ;
	Cliente = 'Bianca',
	Filmes = ['Edukators'] ;
	Cliente = 'Bob',
	Filmes = ['Abril despedaçado', 'Edukators', 'Monty Python: O Sentido da Vida'] ;
	Cliente = 'Maria',
	Filmes = ['Edukators', 'Monty Python: O Sentido da Vida'] ;
	Cliente = 'Tom',
	Filmes = ['Abril despedaçado', 'Edukators', 'Lavoura Arcaica'] ;
	Cliente = 'Zeca',
	Filmes = ['Lavoura Arcaica'].
*/

% S

/*
	?- findall(F, (locacao(c2, Idvd1, _), locacao(c3, Idvd2, _), dvd(Idvd1, IdF, _), dvd(Idvd2, IdF, _), filme(IdF, F, _, _, _)), L).
	L = ['Lavoura Arcaica'].
*/

% T

/*
	?- findall(T, (filme(F, T, _, _, _), dvd(Idvd, F, _), locacao(c1, Idvd, _), not((dvd(Idvd1, F, _), locacao(c6, Idvd1, _)))), L).
	L = ['Abril despedaçado'].
*/

% U

/*
	?- findall(T, (filme(F, T, _, _, _), not((locacao(c1, IdvdB, _), locacao(c6, IdvdM, _), (dvd(IdvdB, F, _); dvd(IdvdM, F, _))))), L).
	L = ['Lavoura Arcaica', 'Lisbela e o Prisioneira', 'Diários de motocicleta'].	
*/